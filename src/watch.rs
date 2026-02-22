use crate::app_state::AppState;
use crate::compiler::Compiler;
use crate::term;
use anyhow::Result;
use crossterm::event::{self, Event, KeyCode, KeyModifiers};
use notify_debouncer_mini::{new_debouncer, DebouncedEvent, DebouncedEventKind};
use std::path::Path;
use std::sync::mpsc;
use std::time::Duration;

enum WatchEvent {
    FileChanged,
    Key(KeyCode),
    Quit,
}

pub fn run_watch(
    state: &mut AppState,
    compiler: &Compiler,
    exercises_dir: &Path,
    build_dir: &Path,
) -> Result<()> {
    let (tx, rx) = mpsc::channel();

    // File watcher
    let tx_fs = tx.clone();
    let mut debouncer = new_debouncer(
        Duration::from_millis(500),
        move |res: Result<Vec<DebouncedEvent>, notify::Error>| {
            if let Ok(events) = res {
                for event in events {
                    if event.kind == DebouncedEventKind::Any {
                        let path_str = event.path.to_string_lossy().to_string();
                        if path_str.ends_with(".c") {
                            let _ = tx_fs.send(WatchEvent::FileChanged);
                        }
                    }
                }
            }
        },
    )?;

    debouncer.watcher().watch(
        exercises_dir,
        notify::RecursiveMode::Recursive,
    )?;

    // Enable raw mode for key input
    crossterm::terminal::enable_raw_mode()?;

    // Initial run
    term::clear_screen();
    print_watch_header(state, compiler);
    run_current_exercise(state, compiler, build_dir);
    print_watch_commands();

    // Keyboard reader thread
    let tx_key = tx.clone();
    std::thread::spawn(move || loop {
        if let Ok(Event::Key(key_event)) = event::read() {
            if key_event.modifiers.contains(KeyModifiers::CONTROL)
                && key_event.code == KeyCode::Char('c')
            {
                let _ = tx_key.send(WatchEvent::Quit);
                break;
            }
            let _ = tx_key.send(WatchEvent::Key(key_event.code));
        }
    });

    loop {
        match rx.recv() {
            Ok(WatchEvent::FileChanged) => {
                term::clear_screen();
                print_watch_header(state, compiler);
                run_current_exercise(state, compiler, build_dir);
                print_watch_commands();
            }
            Ok(WatchEvent::Key(KeyCode::Char('n'))) => {
                // Mark current as done and advance
                if let Some(name) = state.current_exercise().map(|e| e.name().to_string()) {
                    state.mark_done(&name);
                }
                if state.all_done() {
                    term::clear_screen();
                    println!("\r");
                    term::print_success("All exercises completed! Congratulations!");
                    println!("\r");
                    break;
                }
                state.next_pending();
                state.save()?;
                term::clear_screen();
                print_watch_header(state, compiler);
                run_current_exercise(state, compiler, build_dir);
                print_watch_commands();
            }
            Ok(WatchEvent::Key(KeyCode::Char('h'))) => {
                // Show hint
                if let Some(exercise) = state.current_exercise() {
                    let hint = exercise.hint().to_string();
                    term::clear_screen();
                    print_watch_header(state, compiler);
                    println!("\r");
                    term::print_header("Hint:");
                    println!("\r");
                    for line in hint.lines() {
                        println!("  {line}\r");
                    }
                    println!("\r");
                    print_watch_commands();
                }
            }
            Ok(WatchEvent::Key(KeyCode::Char('l'))) => {
                // List exercises
                term::clear_screen();
                println!("\r");
                term::print_header("Exercises:");
                println!("\r");
                for (i, ex) in state.exercises.iter().enumerate() {
                    let status = if state.is_done(ex.name()) {
                        "✓"
                    } else if i == state.current_index {
                        "→"
                    } else {
                        " "
                    };
                    println!("  {status} {}\r", ex.name());
                }
                println!("\r");
                print_watch_commands();
            }
            Ok(WatchEvent::Key(KeyCode::Char('r'))) => {
                // Re-run current exercise
                term::clear_screen();
                print_watch_header(state, compiler);
                run_current_exercise(state, compiler, build_dir);
                print_watch_commands();
            }
            Ok(WatchEvent::Key(KeyCode::Char('q'))) | Ok(WatchEvent::Quit) => {
                break;
            }
            Ok(_) => {}
            Err(_) => break,
        }
    }

    crossterm::terminal::disable_raw_mode()?;
    state.save()?;
    Ok(())
}

fn print_watch_header(state: &AppState, compiler: &Compiler) {
    let (done, total) = state.progress();
    println!("\r");
    term::print_header(&format!("clings 🔧 [{}]", compiler.kind()));
    term::print_progress(done, total);
    println!("\r");
}

fn print_watch_commands() {
    println!("\r");
    let mut stdout = std::io::stdout();
    let _ = crossterm::execute!(
        stdout,
        crossterm::style::SetForegroundColor(crossterm::style::Color::DarkGrey)
    );
    println!("  [n] next  [h] hint  [l] list  [r] re-run  [q] quit\r");
    let _ = crossterm::execute!(
        stdout,
        crossterm::style::SetAttribute(crossterm::style::Attribute::Reset)
    );
}

fn run_current_exercise(state: &AppState, compiler: &Compiler, build_dir: &Path) {
    let exercise = match state.current_exercise() {
        Some(e) => e,
        None => {
            term::print_warning("No exercises found.");
            return;
        }
    };

    println!("  Exercise: {}\r", exercise.name());
    println!("  File: {}\r", exercise.path.display());
    println!("\r");

    if !exercise.exists() {
        term::print_error(&format!(
            "Exercise file not found: {}",
            exercise.path.display()
        ));
        return;
    }

    match exercise.verify(compiler, build_dir) {
        Ok(result) => {
            if result.success {
                term::print_success(&format!("{} compiled and ran successfully!", exercise.name()));
                if !result.output.is_empty() {
                    term::print_stage_output("Program", &result.output);
                }
                println!("\r");
                term::print_info("Press 'n' to move to the next exercise.");
            } else {
                term::print_error(&format!("{} failed at stage: {}", exercise.name(), result.stage));
                term::print_stage_output(result.stage, &result.output);
            }
        }
        Err(e) => {
            term::print_error(&format!("Error verifying {}: {e}", exercise.name()));
        }
    }
}
