# CMSIS + FreeRTOS + EventGroups + Timers

TARGET = cfet

GCC = arm-none-eabi-gcc
CP = arm-none-eabi-objcopy


CFLAGS = -std=gnu99 -g -O2 -Wall -T ./Startup/STM32F103XB_FLASH.ld
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m3
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += --specs=nosys.specs


INC_DIRS = ./CMSIS/inc/
INC_DIRS += ./Code/inc/
INC_DIRS += ./FreeRTOS/include/
INC_DIRS += .
INC = $(addprefix -I,$(INC_DIRS))

SRC_DIRS = ./Code/src/
SRC_DIRS += ./CMSIS/src/
SRC_DIRS += ./Startup/
SRC_DIRS += ./FreeRTOS/
SRC_DIRS += .

SRCS += ./Code/src/main.c
SRCS += ./CMSIS/src/system_stm32f1xx.c
SRCS += ./FreeRTOS/event_groups.c
SRCS += ./FreeRTOS/heap_1.c
SRCS += ./FreeRTOS/list.c
SRCS += ./FreeRTOS/port.c
SRCS += ./FreeRTOS/tasks.c
SRCS += ./Startup/startup_stm32f103xb.s

.PHONY: $(TARGET)

$(TARGET): $(TARGET).elf

$(TARGET).elf: $(SRCS)
	$(GCC) $(INC) $(CFLAGS) $^ -o $@
	$(CP) -O ihex $(TARGET).elf $(TARGET).hex
	$(CP) -O binary $(TARGET).elf $(TARGET).bin

clean:
	rm -f *.o $(TARGET).elf $(TARGET).hex $(TARGET).bin

flash:
	st-flash write $(TARGET).bin 0x8000000


