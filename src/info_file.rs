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
    /// Single hint (legacy format, still supported)
    #[serde(default)]
    pub hint: Option<String>,
    /// Progressive hints array (new format)
    #[serde(default)]
    pub hints: Option<Vec<String>>,
}

impl ExerciseInfo {
    /// Get all hints as a slice, merging both formats.
    /// If `hints` array is set, use that. Otherwise wrap `hint` string.
    pub fn get_hints(&self) -> Vec<&str> {
        if let Some(hints) = &self.hints {
            hints.iter().map(|s| s.as_str()).collect()
        } else if let Some(hint) = &self.hint {
            if hint.is_empty() {
                vec![]
            } else {
                vec![hint.as_str()]
            }
        } else {
            vec![]
        }
    }
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
