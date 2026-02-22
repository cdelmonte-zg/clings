use anyhow::{Context, Result};
use serde::Deserialize;
use std::path::Path;

fn default_true() -> bool {
    true
}

#[derive(Deserialize)]
pub struct InfoFile {
    pub format_version: u32,
    #[serde(default)]
    #[allow(dead_code)]
    pub default_compiler: Option<String>,
    #[serde(default)]
    pub welcome_message: Option<String>,
    #[serde(default)]
    #[allow(dead_code)]
    pub final_message: Option<String>,
    pub exercises: Vec<ExerciseInfo>,
}

#[derive(Deserialize, Clone)]
pub struct ExerciseInfo {
    pub name: String,
    pub dir: String,
    #[serde(default = "default_true")]
    pub test: bool,
    #[serde(default)]
    pub sanitizers: bool,
    #[serde(default)]
    pub hint: String,
}

impl InfoFile {
    pub fn parse(path: &Path) -> Result<Self> {
        let content = std::fs::read_to_string(path)
            .with_context(|| format!("Failed to read {}", path.display()))?;
        let info: InfoFile =
            toml::from_str(&content).with_context(|| format!("Failed to parse {}", path.display()))?;

        if info.format_version != 1 {
            anyhow::bail!(
                "Unsupported info.toml format_version: {}",
                info.format_version
            );
        }

        Ok(info)
    }
}
