# WinAVR cross-compiler toolchain is used here
CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size
DUMP = avr-objdump
DUDE = avrdude

#avr-objdump -h -S test_blink.elf > test_blink.lss

# If you are not using ATtiny85 and the USBtiny programmer, 
# update the lines below to match your configuration
CFLAGS = -Wall -Os -mmcu=attiny13 -DF_CPU=128000UL
OBJFLAGS = -j .text -j .data -O ihex
DUMPFLAGS = -h -S
DUDEFLAGS = -p attiny13 -c usbasp -v

# Object files for the firmware (usbdrv/oddebug.o not strictly needed I think)
OBJECTS = main.o

# By default, build the firmware and command-line client, but do not flash
all: main.hex

# With this, you can flash the firmware by just typing "make flash" on command-line
flash: main.hex
	$(DUDE) $(DUDEFLAGS) -U flash:w:$<

eeprom: main.eep
	$(DUDE) $(DUDEFLAGS) -U eeprom:w:$<

# Housekeeping if you want it
clean:
	$(RM) *.o *.hex *.elf

# From .elf file to .hex
%.hex: %.elf
	$(OBJCOPY) $(OBJFLAGS) $< $@

# Main.elf requires additional objects to the firmware, not just main.o
main.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@
	$(DUMP) $(DUMPFLAGS) $@ > $@.lss
	$(SIZE) $@
# Without this dependance, .o files will not be recompiled if you change 
# the config! I spent a few hours debugging because of this...
# $(OBJECTS): usbdrv/usbconfig.h

# From C source to .o object file
%.o: %.c	
	$(CC) $(CFLAGS) -c $< -o $@

# From assembler source to .o object file
%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
