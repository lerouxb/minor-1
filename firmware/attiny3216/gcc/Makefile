
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname), Darwin)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
src

# List the object files
OBJS +=  \
src/Tiny4kOLED.o \
src/ssd1306_128x32.o \
src/adc_basic.o \
src/slpctrl.o \
src/bod.o \
atmel_start.o \
src/cpuint.o \
src/vref.o \
src/i2c_simple_master.o \
src/clkctrl.o \
driver_isr.o \
src/usart_basic.o \
src/i2c_types.o \
main.o \
src/dac_basic.o \
src/i2c_master.o \
src/tca.o \
src/protected_io.o \
src/driver_init.o \
src/waveforms.o

OBJS_AS_ARGS +=  \
"src/Tiny4kOLED.o" \
"src/ssd1306_128x32.o" \
"src/test.o" \
"src/adc_basic.o" \
"src/slpctrl.o" \
"src/bod.o" \
"atmel_start.o" \
"src/cpuint.o" \
"src/vref.o" \
"src/i2c_simple_master.o" \
"src/clkctrl.o" \
"driver_isr.o" \
"src/usart_basic.o" \
"src/i2c_types.o" \
"main.o" \
"src/dac_basic.o" \
"src/i2c_master.o" \
"src/tca.o" \
"src/protected_io.o" \
"src/driver_init.o" \
"src/waveforms.o"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"src/Tiny4kOLED.d" \
"src/ssd1306_128x32.d" \
"src/test.d" \
"src/i2c_simple_master.d" \
"src/tca.d" \
"driver_isr.d" \
"src/clkctrl.d" \
"src/cpuint.d" \
"src/adc_basic.d" \
"src/vref.d" \
"main.d" \
"src/bod.d" \
"src/slpctrl.d" \
"src/i2c_master.d" \
"atmel_start.d" \
"src/driver_init.d" \
"src/i2c_types.d" \
"src/protected_io.d" \
"src/dac_basic.d" \
"src/usart_basic.d" \
"src/waveforms.d"

MAKEFILE_DIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
OUTPUT_FILE_NAME :=AtmelStart
QUOTE := "
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf

vpath %.c ../
vpath %.cc ../
vpath %.s ../
vpath %.S ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)

# Linker target




$(OUTPUT_FILE_PATH): $(OBJS)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker
	$(QUOTE)avr-gcc$(QUOTE) -o $(OUTPUT_FILE_NAME).elf $(OBJS_AS_ARGS)  -Wl,--start-group -lm -Wl,--end-group \
-Wl,-Map="$(OUTPUT_FILE_NAME).map" -Wl,--gc-sections \
-mmcu=attiny3216 -B "$(MAKEFILE_DIR)../gcc/dev/attiny3216" \
-L"../gcc/dev/attiny3216/avrxmega3"  \
-l:libattiny3216.a 
	@echo Finished building target: $@

	"avr-objcopy" -O binary "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).bin"
	"avr-objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures \
        "$(OUTPUT_FILE_NAME).elf" "$(OUTPUT_FILE_NAME).hex"
	"avr-objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma \
        .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME).elf" \
        "$(OUTPUT_FILE_NAME).eep" || exit 0
	"avr-objdump" -h -S "$(OUTPUT_FILE_NAME).elf" > "$(OUTPUT_FILE_NAME).lss"
	"avr-size" "$(OUTPUT_FILE_NAME).elf"

# Compiler target(s)




%.o: %.c
	@echo Building file: $<
	@echo AVR/GNU C Compiler
	$(QUOTE)avr-gcc$(QUOTE) -x c -DF_CPU=10000000 -DDEBUG -Os -ffunction-sections -g3 -Wall -c -std=gnu99 \
-mmcu=attiny3216 -B "$(MAKEFILE_DIR)../gcc/dev/attiny3216" \
    -D__tiny3216__ \
-I"../config" -I"../include" -I"../utils" -I"../utils/assembler" -I"../" -I"../include"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

	@echo Finished building: $<

%.o: %.cc
	@echo Building file: $<
	@echo AVR/GNU C++ Compiler
	$(QUOTE)avr-g++$(QUOTE) -x c++ -DF_CPU=10000000 -DDEBUG -Os -ffunction-sections -g3 -Wall -c -std=c++14 \
-mmcu=attiny3216 -B "$(MAKEFILE_DIR)../gcc/dev/attiny3216" \
    -D__tiny3216__ \
-I"../config" -I"../include" -I"../utils" -I"../utils/assembler" -I"../" -I"../include"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo AVR/GNU Assembler
	$(QUOTE)avr-gcc$(QUOTE) -x assembler-with-cpp -DF_CPU=10000000 -DDEBUG -Os -ffunction-sections -g3 -Wall -c -std=gnu99 \
-mmcu=attiny3216 -B "$(MAKEFILE_DIR)../gcc/dev/attiny3216" \
    -D__tiny3216__ \
-I"../config" -I"../include" -I"../utils" -I"../utils/assembler" -I"../" -I"../include"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo AVR/GNU Preprocessing Assembler
	$(QUOTE)avr-gcc$(QUOTE) -x assembler-with-cpp -DF_CPU=10000000 -DDEBUG -Os -ffunction-sections -g3 -Wall -c -std=gnu99 \
-mmcu=attiny3216 -B "$(MAKEFILE_DIR)../gcc/dev/attiny3216" \
    -D__tiny3216__ \
-I"../config" -I"../include" -I"../utils" -I"../utils/assembler" -I"../" -I"../include"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"

	@echo Finished building: $<

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean:
	rm -f $(OBJS_AS_ARGS)
	rm -f $(OUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	rm -f $(OUTPUT_FILE_NAME).a $(OUTPUT_FILE_NAME).hex $(OUTPUT_FILE_NAME).bin \
        $(OUTPUT_FILE_NAME).lss $(OUTPUT_FILE_NAME).eep $(OUTPUT_FILE_NAME).map \
        $(OUTPUT_FILE_NAME).srec

flash:  $(OUTPUT_FILE_NAME).hex
	pyupdi -d tiny3216 -c /dev/cu.usbserial-DA01L36P -f AtmelStart.hex
