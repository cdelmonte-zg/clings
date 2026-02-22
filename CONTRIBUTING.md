# Contributing to clings

Thanks for your interest in contributing!

## Adding an exercise

1. Create the exercise file in `exercises/<topic_dir>/<name>.c`
2. Create the matching solution in `solutions/<topic_dir>/<name>.c`
3. Add an entry in `info.toml` with progressive hints

### Naming

- Topic directories: `NN_topic` (e.g. `01_pointers`, `06_strings`)
- Exercise files: `<topic>N.c` (e.g. `pointers1.c`, `pointers2.c`)

### Exercise structure

```c
// Short description of what this exercise teaches.

#include <stdio.h>

// TODO: Fix/implement something
void function_with_bug(void) { ... }

#ifndef TEST
int main(void) {
    // Interactive demo that shows the bug
}
#else
#include "clings_test.h"

TEST(test_name) { ASSERT_EQ(...); }

int main(void) {
    RUN_TEST(test_name);
    TEST_REPORT();
}
#endif
```

### info.toml entry

```toml
[[exercises]]
name = "pointers3"
dir = "01_pointers"
test = true          # compile with -DTEST and run tests
sanitizers = false   # compile with ASan/UBSan
hints = [
    "First hint: the gentlest nudge",
    "Second hint: more specific",
    "Third hint: almost the answer",
]
```

### Requirements for solutions

- Must compile with `-Wall -Wextra -Werror -pedantic -std=c11`
- Must pass with both `gcc` and `clang`
- Must pass with `-fsanitize=address,undefined` when sanitizers are enabled
- Stick to C11 standard -- no POSIX-specific features

## Running tests locally

```bash
cargo test                # Rust unit + integration tests (32 tests)
cargo clippy -- -D warnings

# Verify all C solutions manually
find solutions -name '*.c' | sort | while read -r f; do
  gcc -Wall -Wextra -Werror -pedantic -std=c11 -Iinclude -DTEST -o /tmp/test_bin "$f" && /tmp/test_bin
done
```

## Code style

- **C:** follow the existing style, 4-space indent
- **Rust:** `cargo fmt` + `cargo clippy`
