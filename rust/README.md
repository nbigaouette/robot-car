# Spot

Based on [Rahix/avr-hal](https://github.com/Rahix/avr-hal/blob/b1aedf8/README.md#starting-your-own-project)
and [Dajamante/avr-car](https://github.com/Dajamante/avr-car)

See [Aïssata Maiga - Build your own (Rust-y) robot! — RustFest Global 2020](https://www.youtube.com/watch?v=K2n3uS5BAR8).

See also [The AVR-Rust Guidebook](https://github.com/osx-cross/homebrew-avr)
for some more details (seems outdated).

## Setup

### macOS

```sh
brew install arduino arduino-cli
```

```sh
brew tap osx-cross/avr

brew install avr-binutils avr-gcc avrdude
```

## Compilation

```sh
cargo build --release

file target/avr-unknown-gnu-atmega328/release/spot.elf
```

## Flashing

Due to the [`.cargo/config`](.cargo/config), _run_ cargo command will flash (using
[`uno-runner.sh`](uno-runner.sh)):

```sh
cargo run --release
```
