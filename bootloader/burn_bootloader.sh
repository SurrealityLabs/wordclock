avrdude -c usbtiny -p m328p -F -U lock:w:0x3F:m
avrdude -c usbtiny -p m328p -F -U efuse:w:0x05:m -U hfuse:w:0xda:m -U lfuse:w:0xe2:m
avrdude -c usbtiny -B 10 -p m328p -F -U flash:w:ATmegaBOOT_168_atmega328_pro_8MHz.hex
avrdude -c usbtiny -p m328p -F -U lock:w:0x0F:m
