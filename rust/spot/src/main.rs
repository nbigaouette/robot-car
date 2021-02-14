#![no_std]
#![no_main]

use arduino_uno::{
    hal::{
        clock::MHz16,
        port::{
            mode::{Floating, Input, Output},
            portd::{PD0, PD1, PD2},
        },
        usart::Usart,
    },
    pac::USART0,
    prelude::*,
};
use infrared::{protocols::*, PeriodicReceiver};
use panic_halt as _;

#[arduino_uno::entry]
fn main() -> ! {
    let dp = arduino_uno::Peripherals::take().unwrap();
    let pins = arduino_uno::Pins::new(dp.PORTB, dp.PORTC, dp.PORTD);

    // this is the console. To see the output do (on mac)
    // screen /dev/tty/<your_tty_here> 57600
    // ls /dev/tty* | grep usb --> get the usb connected
    // 57600 is the baud rate
    let mut serial: Usart<USART0, PD0<Input<Floating>>, PD1<Output>, MHz16> =
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
            // 57600.into_baudrate(),
            9600.into_baudrate(),
        );

    ufmt::uwriteln!(&mut serial, "Initializing Arduino Uno...").void_unwrap();

    // Digital pin 13 is also connected to an onboard LED marked "L"
    let mut led = pins.d13.into_output(&pins.ddr);
    led.set_high().void_unwrap();

    const IR_SAMPLERATE: u32 = 20_000;
    let mut ir_receiver: PeriodicReceiver<Nec, PD2<Input<Floating>>> =
    // let mut ir_receiver: PeriodicReceiver<NecDebug, PD2<Input<Floating>>> =
    // let mut ir_receiver: PeriodicReceiver<Nec16, PD2<Input<Floating>>> =
        PeriodicReceiver::new(pins.d2, IR_SAMPLERATE);

    ufmt::uwriteln!(&mut serial, "Arduino Uno initialized, looping...").void_unwrap();
    loop {
        // ufmt::uwriteln!(&mut serial, "Loop...").void_unwrap();
        if let Ok(Some(cmd)) = ir_receiver.poll() {
            // ufmt::uwriteln!(&mut serial, "{:?}", cmd.bits).void_unwrap();
            ufmt::uwriteln!(&mut serial, "{:?} {:?} {:?}", cmd.addr, cmd.cmd, cmd.repeat)
                .void_unwrap();
            // ufmt::uwriteln!(&mut serial, "{:?} {:?}", cmd.address(), cmd.command(),).void_unwrap();
        }

        // 20 kHz == period of 50 mus
        // The delay between polling must match the receiver's sample rate
        arduino_uno::delay_us(50);
    }
}
