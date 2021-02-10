# Spot

See [The AVR-Rust Guidebook](https://github.com/osx-cross/homebrew-avr)
for more details.

## Setup

### macOS

```sh
brew tap osx-cross/avr

brew install avr-binutils avr-gcc avrdude
```

## Compilation

```sh
cargo build -Z build-std=core --target avr-unknown-gnu-atmega328 --release

file target/avr-unknown-gnu-atmega328/release/spot.elf
```
