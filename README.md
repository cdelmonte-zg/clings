```
       _ _
   ___| (_)_ __   __ _ ___
  / __| | | '_ \ / _` / __|
 | (__| | | | | | (_| \__ \
  \___|_|_|_| |_|\__, |___/
                  |___/
```

**Small exercises to learn advanced C concepts.**

Inspired by [rustlings](https://github.com/rust-lang/rustlings) -- fix broken C code,
learn by doing.

## Install

```
cargo install --path .
```

## Quick Start

```
cd clings
clings
```

Each exercise has a bug or TODO for you to fix. Edit the `.c` file in your
editor and clings will automatically recompile and verify your changes.

Commands: `[n]ext` `[h]int` `[l]ist` `[r]e-run` `[q]uit`

## Exercises

| Topic                | What you will learn                              |
|----------------------|--------------------------------------------------|
| 00 Intro             | Getting started, basic program structure          |
| 01 Pointers          | Decay, arithmetic, pointer-size pitfalls          |
| 02 Memory            | malloc/free, leaks, use-after-free, double-free   |
| 03 Undefined Behavior| Signed overflow, strict aliasing, sequence points |
| 04 Preprocessor      | Stringify, token pasting, macro pitfalls          |
| 05 UB Lab            | Hands-on UB experiments with sanitizer feedback   |

## Features

- **Watch mode** -- recompiles on every save, shows errors instantly
- **gcc + clang** -- exercises are verified against both compilers
- **Sanitizers** -- AddressSanitizer and UBSan catch hidden bugs
- **UB Lab** -- dedicated exercises where you trigger real undefined behavior
  and observe how sanitizers detect it

## License

MIT
