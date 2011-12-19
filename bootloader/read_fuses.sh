avrdude -p m328p -c usbtiny -U hfuse:r:./high.txt:s -U lfuse:r:./low.txt:s -U efuse:r:./efuse.txt:s -F
