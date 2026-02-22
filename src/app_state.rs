use crate::exercise::Exercise;
use anyhow::{Context, Result};
use std::collections::HashSet;
use std::path::{Path, PathBuf};

const STATE_FILE: &str = ".clings-state.txt";

pub struct AppState {
    state_path: PathBuf,
    pub exercises: Vec<Exercise>,
    done: HashSet<String>,
    pub current_index: usize,
}

impl AppState {
    pub fn new(exercises: Vec<Exercise>, base_dir: &Path) -> Result<Self> {
        let state_path = base_dir.join(STATE_FILE);
        let mut state = Self {
            state_path,
            exercises,
            done: HashSet::new(),
            current_index: 0,
        };
        state.load()?;
        Ok(state)
    }

    fn load(&mut self) -> Result<()> {
        if !self.state_path.exists() {
            return Ok(());
        }

        let content = std::fs::read_to_string(&self.state_path)
            .with_context(|| "Failed to read state file")?;

        let mut lines = content.lines();

        // Skip header comment
        lines.next();
        // Skip blank line
        lines.next();

        // Current exercise name
        if let Some(current_name) = lines.next() {
            let current_name = current_name.trim();
            if !current_name.is_empty() {
                if let Some(idx) = self
                    .exercises
                    .iter()
                    .position(|e| e.name() == current_name)
                {
                    self.current_index = idx;
                }
            }
        }

        // Skip blank line
        lines.next();

        // Done exercises
        for line in lines {
            let name = line.trim();
            if !name.is_empty() {
                self.done.insert(name.to_string());
            }
        }

        Ok(())
    }

    pub fn save(&self) -> Result<()> {
        let current_name = self
            .exercises
            .get(self.current_index)
            .map(|e| e.name())
            .unwrap_or("");

        let mut content = String::new();
        content.push_str("DON'T EDIT THIS FILE!\n");
        content.push('\n');
        content.push_str(current_name);
        content.push('\n');
        content.push('\n');

        for name in &self.done {
            content.push_str(name);
            content.push('\n');
        }

        std::fs::write(&self.state_path, content).with_context(|| "Failed to write state file")?;
        Ok(())
    }

    pub fn current_exercise(&self) -> Option<&Exercise> {
        self.exercises.get(self.current_index)
    }

    pub fn is_done(&self, name: &str) -> bool {
        self.done.contains(name)
    }

    pub fn mark_done(&mut self, name: &str) {
        self.done.insert(name.to_string());
    }

    pub fn next_pending(&mut self) -> bool {
        let start = self.current_index;
        let len = self.exercises.len();

        for i in 1..=len {
            let idx = (start + i) % len;
            if !self.done.contains(self.exercises[idx].name()) {
                self.current_index = idx;
                return true;
            }
        }
        false
    }

    pub fn all_done(&self) -> bool {
        self.exercises.iter().all(|e| self.done.contains(e.name()))
    }

    pub fn progress(&self) -> (usize, usize) {
        (self.done.len(), self.exercises.len())
    }

    pub fn find_exercise(&self, name: &str) -> Option<usize> {
        self.exercises.iter().position(|e| e.name() == name)
    }

    pub fn reset_exercise(&self, name: &str) -> Result<()> {
        if let Some(idx) = self.find_exercise(name) {
            let exercise = &self.exercises[idx];
            if exercise.solution_path.exists() {
                // We reset by extracting the original - for now just inform the user
                anyhow::bail!(
                    "Reset not yet implemented. Please manually restore {} from the original.",
                    exercise.path.display()
                );
            }
        } else {
            anyhow::bail!("Exercise '{}' not found", name);
        }
        Ok(())
    }
}
