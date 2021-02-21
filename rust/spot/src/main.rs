#![no_std]
#![no_main]
#![feature(abi_avr_interrupt)]

// use core::cell;

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
use ufmt::uwriteln;

// ╔═══════════╦══════════════╦═══════════════════╗
// ║ PRESCALER ║ TIMER_COUNTS ║ Overflow Interval ║
// ╠═══════════╬══════════════╬═══════════════════╣
// ║        8  ║          100 ║            50 mus ║ 50 mus ~ 20 kHz
// ║        8  ║          125 ║          62.5 mus ║
// ║        8  ║          250 ║           125 mus ║
// ║        64 ║          125 ║           500 mus ║
// ║        64 ║          250 ║              1 ms ║
// ║       256 ║          125 ║              2 ms ║
// ║       256 ║          250 ║              4 ms ║
// ║      1024 ║          125 ║              8 ms ║
// ║      1024 ║          250 ║             16 ms ║
// ╚═══════════╩══════════════╩═══════════════════╝
const PRESCALER: u32 = 8;
const TIMER_COUNTS: u32 = 100;
// 16 ms, 62.5 Hz
// const PRESCALER: u32 = 1024;
// const TIMER_COUNTS: u32 = 250;

// const MILLIS_INCREMENT: u32 = PRESCALER * TIMER_COUNTS / 16_000;
// static MILLIS_COUNTER: avr_device::interrupt::Mutex<cell::Cell<u32>> =
//     avr_device::interrupt::Mutex::new(cell::Cell::new(0));

const IR_SAMPLERATE: u32 = 20_000;
// const IR_SAMPLERATE: u32 = 62;

// Pin connected to the receiver
type IRReceiverPin = PD2<Input<Floating>>;

// Our timer. Needs to be accessible in the interrupt handler.
// static mut TIMER: Option<CountDownTimer<TIM2>> = None;
// static mut TIMER: Option<arduino_uno::pac::TC2> = None;

type Receiver = PeriodicReceiver<Nec, IRReceiverPin>;
static mut RECEIVER: Option<Receiver> = None;

type Serial = Usart<USART0, PD0<Input<Floating>>, PD1<Output>, MHz16>;
static mut SERIAL: Option<Serial> = None;

#[arduino_uno::entry]
fn main() -> ! {
    let dp = arduino_uno::Peripherals::take().unwrap();
    let pins = arduino_uno::Pins::new(dp.PORTB, dp.PORTC, dp.PORTD);

    // this is the console. To see the output do (on mac)
    // screen /dev/tty/<your_tty_here> 57600
    // ls /dev/tty* | grep usb --> get the usb connected
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
            57600.into_baudrate(),
            // 9600.into_baudrate(),
        );

    avr_device::interrupt::free(|_cs| unsafe {
        ufmt::uwriteln!(&mut serial, "Initializing Arduino Uno...").void_unwrap();
        SERIAL.replace(serial);
    });

    // Digital pin 13 is also connected to an onboard LED marked "L"
    // let mut led = pins.d13.into_output(&pins.ddr);
    // led.set_high().void_unwrap();

    init_timer(dp.TC0);

    // ================================================================================
    /*
    // From IRremote.cpp, line 276
    cli();
    setup pulse clock timer interrupt
    Prescale /8 (16M/8 = 0.5 microseconds per tick)
    Therefore, the timer interval can range from 0.5 to 128 microseconds
    depending on the reset value (255 to 0)
    TIMER_CONFIG_NORMAL(); // ==> ({ TCCR2A = _BV(WGM21); TCCR2B = _BV(CS20); OCR2A = TIMER_COUNT_TOP; TCNT2 = 0; })
    Timer2 Overflow Interrupt Enable
    TIMER_ENABLE_INTR; ==> (TIMSK2 = _BV(OCIE2A))
    TIMER_RESET; // ==> ∅
    sei();  // enable interrupts
    */

    // let timer2: arduino_uno::pac::TC2 = dp.TC2;
    // avr_device::interrupt::free(|_cs| {
    //     // // Configure the timer in a "Critical Section" (interrupts disabled)
    //     // // -----------------------------
    //     // // TIMER_CONFIG_NORMAL();
    //     // // let timer2: arduino_uno::pac::TC2 = dp.TC2;
    //     // // Timer/Counter2 Control Register A: TCCR2A = _BV(WGM21)
    //     // // timer2.tccr2a.write(|w| w.wgm2().pwm_fast()); // FIXME: Is it really pwm_fast()?
    //     // // Timer/Counter2 Control Register B: TCCR2B = _BV(CS20)
    //     // timer2.tccr2b.write(|w| w.cs2().prescale_8());
    //     // // Timer/Counter2 Output Compare Register A: OCR2A = TIMER_COUNT_TOP
    //     // // timer2.ocr2a.write(|w| w.bits()) // FIXME: Required?
    //     // // Timer/Counter2: TCNT2 = 0
    //     // timer2.tcnt2.write(|w| unsafe { w.bits(0) });
    //     // // -----------------------------
    //     // // Timer2 Overflow Interrupt Enable
    //     // // TIMER_ENABLE_INTR; ==> (TIMSK2 = _BV(OCIE2A))
    //     // timer2.timsk2.write(|w| w.ocie2a().bit(true));
    //     // // -----------------------------

    //     unsafe {
    //         TIMER.replace(timer2);
    //     }
    // });
    // ================================================================================

    // https://jott.se/blog/infrared/
    let ir_receiver: PeriodicReceiver<Nec, PD2<Input<Floating>>> =
        PeriodicReceiver::new(pins.d2, IR_SAMPLERATE);

    // ufmt::uwriteln!(&mut serial, "Arduino Uno initialized, looping...").void_unwrap();
    // loop {
    //     // ufmt::uwriteln!(&mut serial, "Loop...").void_unwrap();
    //     if let Ok(Some(cmd)) = ir_receiver.poll() {
    //         // ufmt::uwriteln!(&mut serial, "{:?}", cmd.bits).void_unwrap();
    //         uwriteln!(&mut serial, "{:?} {:?} {:?}", cmd.addr, cmd.cmd, cmd.repeat).void_unwrap();
    //         // ufmt::uwriteln!(&mut serial, "{:?} {:?}", cmd.address(), cmd.command(),).void_unwrap();
    //     }

    //     // 20 kHz == period of 50 mus
    //     // The delay between polling must match the receiver's sample rate
    //     arduino_uno::delay_us(50);
    // }

    avr_device::interrupt::free(|_cs| unsafe {
        RECEIVER.replace(ir_receiver);
    });

    // Enable interrupts globally
    unsafe { avr_device::interrupt::enable() };

    loop {
        // 20 kHz == period of 50 mus
        // The delay between polling must match the receiver's sample rate
        arduino_uno::delay_us(50);
    }
}

// // See https://blog.rahix.de/005-avr-hal-millis/
// fn read_ir() {
//     // 1. Save the current interrupt state (whether interrupts are enabled or disabled)
//     // 2. Disable interrupts with a cli instruction
//     // 4. Restore interrupt state
//     avr_device::interrupt::free(|_cs| {
//         // TODO: Magic
//         unimplemented!()
//     })
// }

fn init_timer(tc0: arduino_uno::pac::TC0) {
    // https://github.com/Rahix/avr-hal/blob/bfc950428919af96030e66e9b44af2b4574a1ec1/boards/arduino-uno/examples/uno-millis.rs#L36-L54

    // Configure the timer for the above interval (in CTC mode)
    // and enable its interrupt.
    tc0.tccr0a.write(|w| w.wgm0().ctc());
    tc0.ocr0a.write(|w| unsafe { w.bits(TIMER_COUNTS as u8) });
    tc0.tccr0b.write(|w| match PRESCALER {
        8 => w.cs0().prescale_8(),
        64 => w.cs0().prescale_64(),
        256 => w.cs0().prescale_256(),
        1024 => w.cs0().prescale_1024(),
        _ => panic!(),
    });
    tc0.timsk0.write(|w| w.ocie0a().set_bit());

    // // Reset the global millisecond counter
    // avr_device::interrupt::free(|cs| {
    //     MILLIS_COUNTER.borrow(cs).set(0);
    // });
}

#[avr_device::interrupt(atmega328p)]
fn TIMER0_COMPA() {
    avr_device::interrupt::free(|_cs| {
        let receiver = unsafe { RECEIVER.as_mut().unwrap() };
        let serial = unsafe { SERIAL.as_mut().unwrap() };
        // uwriteln!(serial, "Interrupt received!").void_unwrap();

        if let Ok(Some(cmd)) = receiver.poll() {
            // ufmt::uwriteln!(&mut serial, "{:?}", cmd.bits).void_unwrap();
            uwriteln!(serial, "{:?} {:?} {:?}", cmd.addr, cmd.cmd, cmd.repeat).void_unwrap();
            // ufmt::uwriteln!(&mut serial, "{:?} {:?}", cmd.address(), cmd.command(),).void_unwrap();
        }
    })
}
