#Контроллер, установленный на плате. Может быть другим, например atmega328 
DEVICE     = atmega168

#Тактовая частота 16 МГц 
CLOCK      = 16000000

#Команда запуска avrdude. Ее нужно скопировать из Arduino IDE.
AVRDUDE = /Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/bin/avrdude -C/Applications/Arduino.app/Contents/Resources/Java/hardware/tools/avr/etc/avrdude.conf -carduino -P/dev/tty.usbserial-A600dAAQ -b19200 -D -p atmega168

OBJECTS    = main.o

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

clean:
	rm -f main.hex main.elf $(OBJECTS)

main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
