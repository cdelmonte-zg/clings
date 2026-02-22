[![CI](https://github.com/cdelmonte-zg/clings/actions/workflows/ci.yml/badge.svg)](https://github.com/cdelmonte-zg/clings/actions/workflows/ci.yml)

```
       _ _
   ___| (_)_ __   __ _ ___
  / __| | | '_ \ / _` / __|
 | (__| | | | | | (_| \__ \
  \___|_|_|_| |_|\__, |___/
                  |___/
```

**Small exercises to learn advanced C concepts.**
Inspired by [rustlings](https://github.com/rust-lang/rustlings) — fix broken C code, learn by doing.

---

## Demo

![clings exercise](assets/clings-1.png)

![clings success](assets/clings-2.png)

*Edit a `.c` file → save → clings recompiles → read the hint → fix → green.*

---

## Install

```bash
cargo install --path .
```

> **Requirements:** a working `gcc` and/or `clang`, plus the Rust toolchain.

## Quick start

```bash
cd clings
clings
```

Each exercise has a bug or a `TODO` for you to fix.
Edit the `.c` file in your editor and clings will automatically
recompile and verify your changes.

**Interactive commands:**
`[n]ext` · `[p]rev` · `[h]int` · `[l]ist` · `[r]e-run` · `[q]uit`

---

## Exercises (32 total)

| Topic                 | #  | What you will learn                              |
|-----------------------|----|--------------------------------------------------|
| 00 Intro              | 1  | Getting started, basic program structure          |
| 01 Pointers           | 2  | Decay, arithmetic, pointer-size pitfalls          |
| 02 Memory             | 3  | `malloc`/`free`, `realloc`, leaks, double-free    |
| 03 Undefined Behavior | 1  | Signed overflow detection                         |
| 04 Preprocessor       | 1  | Stringify, token pasting, macro pitfalls          |
| 05 UB Lab             | 6  | Hands-on UB experiments with sanitizer feedback   |
| 06 Strings            | 3  | Safe concatenation, tokenizing, parsing           |
| 07 Structs            | 3  | Layout/padding, opaque types, linked lists        |
| 08 Function Pointers  | 3  | Callbacks, generic sort, dispatch tables          |
| 09 Const Correctness  | 3  | `const` parameters, pointer-to-const, immutable API |
| 10 Error Handling     | 3  | Return codes, error propagation, error context    |
| 11 Bitwise            | 3  | Bit counting, packing/unpacking, bit tricks       |

---

## Features

- **Watch mode** — recompiles on every save, shows errors instantly
- **gcc + clang** — exercises are verified against both compilers
- **Sanitizers** — AddressSanitizer and UBSan catch hidden bugs
- **UB Lab** — dedicated exercises where you trigger real undefined behavior
  and observe how sanitizers detect it

## Development

```bash
cargo test           # unit + integration tests
cargo clippy         # lint
```

## License

MIT
