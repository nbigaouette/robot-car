#![no_std]
#![no_main]

use arduino_uno::{
    hal::port::{
        mode::{Floating, Input, Output},
        portb::PB5,
        portd::PD2,
    },
    prelude::*,
    Pins,
};
use panic_halt as _;

enum CommunicationMode {
    Infrared,
    // Bluetooth,
}

struct Car {
    // pins: Pins,
    led_l: PB5<Output>,
    // peripherals: Peripherals,
    infrared: PD2<Input<Floating>>,
}

impl Car {
    fn new() -> Self {
        let dp = arduino_uno::Peripherals::take().unwrap();
        let pins = arduino_uno::Pins::new(dp.PORTB, dp.PORTC, dp.PORTD);

        Self {
            led_l: pins.d13.into_output(&pins.ddr),
            // pins,
            // peripherals: arduino_uno::Peripherals::take().unwrap(),
            // pins: arduino_uno::Pins::new(dp.PORTB, dp.PORTC, dp.PORTD)
            // infrared: PD2<Input<Floating>>
            infrared: pins.d2,
        }
    }
}

#[arduino_uno::entry]
fn main() -> ! {
    // let dp = arduino_uno::Peripherals::take().unwrap();

    // let mut pins = arduino_uno::Pins::new(dp.PORTB, dp.PORTC, dp.PORTD);

    // // Digital pin 13 is also connected to an onboard LED marked "L"
    // let mut led = pins.d13.into_output(&mut pins.ddr);

    // let mut infrared = pins.d2;

    let mut car = Car::new();

    car.led_l.set_high().void_unwrap();

    loop {
        car.led_l.toggle().void_unwrap();
        arduino_uno::delay_ms(200);
        car.led_l.toggle().void_unwrap();
        arduino_uno::delay_ms(200);
        car.led_l.toggle().void_unwrap();
        arduino_uno::delay_ms(200);
        car.led_l.toggle().void_unwrap();
        arduino_uno::delay_ms(800);
    }
}
