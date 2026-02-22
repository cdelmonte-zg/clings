# Contributing to clings

Thanks for your interest in contributing!

## Adding exercises

Each exercise lives in `exercises/<topic_dir>/<name>.c` with a matching solution
in `solutions/<topic_dir>/<name>.c`.

Exercise structure:

```c
// Short description of what this exercise teaches.

#include <stdio.h>

// TODO: Fix/implement something
void function_with_bug(void) { ... }

#ifndef TEST
int main(void) {
    // Interactive demo
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

Requirements:

- Exercises must compile with `-Wall -Wextra -Werror -pedantic -std=c11`
- Solutions must pass with both gcc and clang
- Solutions must pass with `-fsanitize=address,undefined` when sanitizers are enabled
- Stick to C11 standard -- no POSIX-specific features
- Add an entry in `info.toml` with progressive hints

## Running tests locally

```
cargo build
cargo clippy -- -D warnings

# Verify all solutions
find solutions -name '*.c' | while read -r f; do
  gcc -Wall -Wextra -Werror -pedantic -std=c11 -Iinclude -DTEST -o /tmp/test_bin "$f" && /tmp/test_bin
done
```

## Code style

- C: follow the existing style, 4-space indent
- Rust: `cargo fmt` + `cargo clippy`
