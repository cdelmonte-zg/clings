mod app_state;
mod compiler;
mod exercise;
mod info_file;
mod term;
mod watch;

use anyhow::{Context, Result};
use app_state::AppState;
use clap::{Parser, Subcommand};
use compiler::{Compiler, CompilerKind};
use exercise::Exercise;
use info_file::InfoFile;
use std::path::{Path, PathBuf};

#[derive(Parser)]
#[command(name = "clings", version, about = "Small exercises to learn advanced C concepts")]
struct Cli {
    #[command(subcommand)]
    command: Option<Commands>,

    /// Compiler to use (gcc or clang)
    #[arg(long, global = true, default_value = "gcc")]
    compiler: String,
}

#[derive(Subcommand)]
enum Commands {
    /// Run a specific exercise
    Run {
        /// Exercise name
        name: Option<String>,
    },
    /// Show hint for an exercise (use --level N to reveal up to hint N)
    Hint {
        /// Exercise name (defaults to current)
        name: Option<String>,
        /// Hint level to show (1 = first hint, 2 = first two, etc.)
        #[arg(short, long, default_value = "1")]
        level: usize,
    },
    /// List all exercises and their status
    List,
    /// Verify all exercises
    Verify,
    /// Reset progress (start from scratch)
    Reset,
}

fn resolve_base_dir() -> Result<PathBuf> {
    let mut dir = std::env::current_dir()?;
    loop {
        if dir.join("info.toml").exists() {
            return Ok(dir);
        }
        if !dir.pop() {
            break;
        }
    }
    anyhow::bail!(
        "Could not find info.toml. Are you in a clings project directory?"
    )
}

fn load_exercises(info: &InfoFile, base_dir: &Path) -> Vec<Exercise> {
    let exercises_dir = base_dir.join("exercises");
    let solutions_dir = base_dir.join("solutions");
    info.exercises
        .iter()
        .map(|ei| Exercise::new(ei.clone(), &exercises_dir, &solutions_dir))
        .collect()
}

fn main() -> Result<()> {
    let cli = Cli::parse();

    let base_dir = resolve_base_dir().context(
        "Could not find clings project. Make sure you're inside the clings directory.",
    )?;

    let info = InfoFile::parse(&base_dir.join("info.toml"))?;

    let compiler_kind = match cli.compiler.to_lowercase().as_str() {
        "gcc" => CompilerKind::Gcc,
        "clang" => CompilerKind::Clang,
        other => anyhow::bail!("Unknown compiler: {other}. Use 'gcc' or 'clang'."),
    };

    let compiler = Compiler::new(compiler_kind, &base_dir)?;
    let exercises = load_exercises(&info, &base_dir);
    let build_dir = base_dir.join("target").join("clings");
    let exercises_dir = base_dir.join("exercises");
    let mut state = AppState::new(exercises, &base_dir)?;

    match cli.command {
        None => {
            // Default: watch mode
            let welcome = info.welcome_message.as_deref();
            watch::run_watch(&mut state, &compiler, &exercises_dir, &build_dir, welcome)?;
        }
        Some(Commands::Run { name }) => {
            let name = name.unwrap_or_else(|| {
                state
                    .current_exercise()
                    .map(|e| e.name().to_string())
                    .unwrap_or_default()
            });

            let idx = state
                .find_exercise(&name)
                .context(format!("Exercise '{name}' not found"))?;

            let exercise = &state.exercises[idx];
            println!();
            term::print_header(&format!("Running: {}", exercise.name()));
            println!();

            let result = exercise.verify(&compiler, &build_dir)?;
            if result.success {
                term::print_success(&format!("{} passed!", exercise.name()));
                if !result.output.is_empty() {
                    term::print_stage_output("Output", &result.output);
                }
            } else {
                term::print_error(&format!(
                    "{} failed at stage: {}",
                    exercise.name(),
                    result.stage
                ));
                term::print_stage_output(result.stage, &result.output);
                std::process::exit(1);
            }
        }
        Some(Commands::Hint { name, level }) => {
            let name = name.unwrap_or_else(|| {
                state
                    .current_exercise()
                    .map(|e| e.name().to_string())
                    .unwrap_or_default()
            });

            let idx = state
                .find_exercise(&name)
                .context(format!("Exercise '{name}' not found"))?;

            let exercise = &state.exercises[idx];
            let hints = exercise.hints();

            println!();
            if hints.is_empty() {
                term::print_warning(&format!("No hints available for {}.", exercise.name()));
            } else {
                let show_up_to = level.min(hints.len());
                for (i, hint) in hints.iter().take(show_up_to).enumerate() {
                    term::print_header(&format!(
                        "Hint {} of {} for {}:",
                        i + 1,
                        hints.len(),
                        exercise.name()
                    ));
                    println!();
                    for line in hint.lines() {
                        println!("  {line}");
                    }
                    println!();
                }
                if show_up_to < hints.len() {
                    term::print_info(&format!(
                        "Use --level {} to see the next hint.",
                        show_up_to + 1
                    ));
                }
            }
            println!();
        }
        Some(Commands::List) => {
            println!();
            term::print_header("Exercises");
            println!();
            let (done, total) = state.progress();
            term::print_progress(done, total);
            println!();

            let mut current_dir = String::new();
            for (i, exercise) in state.exercises.iter().enumerate() {
                if exercise.info.dir != current_dir {
                    current_dir = exercise.info.dir.clone();
                    println!("  {current_dir}/");
                }
                let status = if state.is_done(exercise.name()) {
                    "✓"
                } else if i == state.current_index {
                    "→"
                } else {
                    " "
                };
                println!("    {status} {}", exercise.name());
            }
            println!();
        }
        Some(Commands::Reset) => {
            state.reset()?;
            println!();
            term::print_success("Progress reset. Starting fresh!");
            println!();
        }
        Some(Commands::Verify) => {
            println!();
            term::print_header("Verifying all exercises...");
            println!();

            let mut all_passed = true;
            for exercise in &state.exercises {
                if !exercise.exists() {
                    term::print_warning(&format!("{}: file not found", exercise.name()));
                    continue;
                }
                match exercise.verify(&compiler, &build_dir) {
                    Ok(result) => {
                        if result.success {
                            term::print_success(exercise.name());
                        } else {
                            term::print_error(&format!(
                                "{} (failed at: {})",
                                exercise.name(),
                                result.stage
                            ));
                            all_passed = false;
                        }
                    }
                    Err(e) => {
                        term::print_error(&format!("{}: {e}", exercise.name()));
                        all_passed = false;
                    }
                }
            }

            println!();
            if all_passed {
                term::print_success("All exercises passed!");
            } else {
                term::print_error("Some exercises failed.");
                std::process::exit(1);
            }
        }
    }

    Ok(())
}
