use anyhow::{Context, Result};
use std::path::{Path, PathBuf};
use std::process::Command;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CompilerKind {
    Gcc,
    Clang,
}

impl CompilerKind {
    pub fn command_name(self) -> &'static str {
        match self {
            CompilerKind::Gcc => "gcc",
            CompilerKind::Clang => "clang",
        }
    }
}

impl std::fmt::Display for CompilerKind {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(self.command_name())
    }
}

pub struct CompileResult {
    pub success: bool,
    pub output: String,
}

pub struct Compiler {
    kind: CompilerKind,
    include_dir: PathBuf,
}

impl Compiler {
    pub fn new(kind: CompilerKind, base_dir: &Path) -> Result<Self> {
        let name = kind.command_name();
        let status = Command::new(name)
            .arg("--version")
            .stdout(std::process::Stdio::null())
            .stderr(std::process::Stdio::null())
            .status()
            .with_context(|| format!("{name} not found. Please install it."))?;

        if !status.success() {
            anyhow::bail!("{name} --version failed");
        }

        Ok(Self {
            kind,
            include_dir: base_dir.join("include"),
        })
    }

    pub fn kind(&self) -> CompilerKind {
        self.kind
    }

    fn include_flag(&self) -> String {
        format!("-I{}", self.include_dir.display())
    }

    fn base_args(&self) -> Vec<String> {
        vec![
            self.include_flag(),
            "-Wall".into(),
            "-Wextra".into(),
            "-Werror".into(),
            "-pedantic".into(),
            "-std=c11".into(),
            "-g".into(),
        ]
    }

    pub fn compile(&self, source: &Path, output: &Path) -> Result<CompileResult> {
        let mut args = self.base_args();
        args.push("-o".into());
        args.push(output.to_str().unwrap().into());
        args.push(source.to_str().unwrap().into());

        self.run_compiler(&args)
    }

    pub fn compile_with_tests(&self, source: &Path, output: &Path) -> Result<CompileResult> {
        let mut args = self.base_args();
        args.push("-DTEST".into());
        args.push("-o".into());
        args.push(output.to_str().unwrap().into());
        args.push(source.to_str().unwrap().into());

        self.run_compiler(&args)
    }

    pub fn compile_with_sanitizers(&self, source: &Path, output: &Path) -> Result<CompileResult> {
        let args = vec![
            self.include_flag(),
            "-fsanitize=address,undefined".into(),
            "-fno-sanitize-recover=all".into(),
            "-g".into(),
            "-std=c11".into(),
            "-o".into(),
            output.to_str().unwrap().into(),
            source.to_str().unwrap().into(),
        ];

        self.run_compiler(&args)
    }

    fn run_compiler(&self, args: &[String]) -> Result<CompileResult> {
        let output = Command::new(self.kind.command_name())
            .args(args)
            .output()
            .with_context(|| format!("Failed to run {}", self.kind))?;

        let mut combined = String::new();
        if !output.stdout.is_empty() {
            combined.push_str(&String::from_utf8_lossy(&output.stdout));
        }
        if !output.stderr.is_empty() {
            combined.push_str(&String::from_utf8_lossy(&output.stderr));
        }

        Ok(CompileResult {
            success: output.status.success(),
            output: combined,
        })
    }
}
