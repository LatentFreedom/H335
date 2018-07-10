OBJS = main.o listsum_recursive_asm.o

ELF=$(notdir $(CURDIR)).elf
TOOLROOT=/l/arm2/devtools/bin

CC=$(TOOLROOT)/arm-none-eabi-gcc
LD=$(TOOLROOT)/arm-none-eabi-gcc
AR=$(TOOLROOT)/arm-none-eabi-ar
AS=$(TOOLROOT)/arm-none-eabi-as
GDB=$(TOOLROOT)/arm-none-eabi-gdb
QEMU=$(TOOLROOT)/qemu-system-arm
OBJDUMP=$(TOOLROOT)/arm-none-eabi-objdump

CFLAGS = -g -mcpu=cortex-m0 -mthumb -I..
LDFLAGS = --specs=rdimon.specs -lc -lrdimon

$(ELF) : $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

.s.o : 
	$(AS) $(CFLAGS) -g $< -o $@
	$(OBJDUMP) -d $@ > $(@:.o=.lst)

.c.o : 
	$(CC) $(CFLAGS) -c $< -o $@

test : $(ELF)
	qemu-system-arm  -machine versatilepb -cpu cortex-m3 -nographic \
                   -monitor null -serial null \
                   -semihosting -kernel $<

debug: $(ELF)
	qemu-system-arm -machine versatilepb -cpu cortex-m3 -nographic -monitor null \
                 -serial null -semihosting -kernel \
                 $< -gdb tcp::51234 -S &
	arm-none-eabi-gdb $< --command=../main.gdb


gdb: $(ELF)
	qemu-system-arm -machine versatilepb -cpu cortex-m3 -nographic -monitor null \
                 -serial null -semihosting -kernel \
                 $< -gdb tcp::51234 -S 

clean: 
	rm -f $(OBJS) $(ELF) $(OBJS:.o=.lst)


