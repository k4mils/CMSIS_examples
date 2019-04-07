TARGET = main

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
SE=arm-none-eabi-size
SF=st-flash

INCLUDES = ./CMSIS/inc/
INCLUDES += ./Code/inc/
INCLUDES += .

INC = $(addprefix -I,$(INCLUDES))

CFLAGS += -std=gnu99 -g -O2 -Wall -T ./Startup/STM32F103XB_FLASH.ld
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m3
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += --specs=nosys.specs

SRCS = ./Code/src/main.c
SRCS += ./CMSIS/src/system_stm32f1xx.c
SRCS += ./Startup/startup_stm32f103xb.s

.PHONY: $(TARGET)

$(TARGET): $(TARGET).elf

$(TARGET).elf: $(SRCS)
	$(CC) $(INC) $(CFLAGS) $^ -o $@
	$(CP) -O ihex $(TARGET).elf $(TARGET).hex
	$(CP) -O binary $(TARGET).elf $(TARGET).bin

clean:
		rm -f *.o $(TARGET).elf $(TARGET).hex $(TARGET).bin

flash:
	$(SF) write $(TARGET).bin 0x8000000
