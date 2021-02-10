#!/bin/sh

# https://electronics.stackexchange.com/questions/33433/how-to-read-the-current-program-from-an-arduino

# macOS
port="/dev/tty.usbmodem142101"

baud_rate="115200"

backup_file="arduino_uno_flash_`date +%Y%m%d_%Hh%M`.hex"

avrdude -p atmega328p -c arduino -b ${baud_rate} -P ${port} -U flash:r:"${backup_file}":r

shasum -a 256 ${backup_file} > ${backup_file}.sha256
