#![no_std]
#![no_main]

use arduino_uno::{
    hal::{
        clock::MHz16,
        port::{
            mode::{Floating, Input, Output},
            portb::PB5,
            portd::{PD0, PD1, PD2},
        },
        usart::{Baudrate, Usart},
    },
    pac::USART0,
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
    serial: Usart<USART0, PD0<Input<Floating>>, PD1<Output>, MHz16>,
}

impl Car {
    fn new() -> Self {
        let dp = arduino_uno::Peripherals::take().unwrap();
        let mut pins = arduino_uno::Pins::new(dp.PORTB, dp.PORTC, dp.PORTD);

        // this is the console. To see the output do (on mac)
        // screen /dev/tty/<your_tty_here> 57600
        // ls /dev/tty* | grep usb --> get the usb connected
        // 57600 is the baud rate
        let serial: Usart<USART0, PD0<Input<Floating>>, PD1<Output>, MHz16> =
            arduino_uno::Serial::new(
                // protocol to communicate bytes in 2 directions
                // USART0 is moved to serial, serial becomes the new owner
                // https://rahix.github.io/avr-hal/atmega328p_hal/usart/struct.Usart0.html
                dp.USART0,
                // the values below correspond to :
                // rx: receive pin (hardwired into the MCU)
                // tx : PD1 is the "hardcoded output"
                // the ownership is moved by writing explicitly input, output is enforced at compile time,
                pins.d0,
                pins.d1.into_output(&pins.ddr),
                // other well known baud rates are possible (9600)
                57600.into_baudrate(),
            );

        Self {
            led_l: pins.d13.into_output(&pins.ddr),
            // pins,
            // peripherals: arduino_uno::Peripherals::take().unwrap(),
            // pins: arduino_uno::Pins::new(dp.PORTB, dp.PORTC, dp.PORTD)
            // infrared: PD2<Input<Floating>>
            infrared: pins.d2,
            serial,
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

    ufmt::uwriteln!(&mut car.serial, "Hello world!").void_unwrap();

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
