REPY := $(shell realpath $(REPY_PATH) 2>/dev/null || echo $(REPY_PATH))
CC := $(REPY)/sdk/toolchain/linux_x86_glibc/bin/x86_64-nacl-gcc
LD := $(CC)
LDLIBS := -llogger
RT_LIBS_PATH := $(REPY)/lib/glibc
LIBS_PATH := $(REPY)/sdk/toolchain/linux_x86_glibc/nacl64/lib64
CPPFLAGS += -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1
CPPFLAGS += -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600
CFLAGS += -c -std=gnu99 -std=gnu99 -Wall -Wextra -pedantic -m64
CFLAGS += -Wno-long-long -Wno-unused-parameter -Wno-missing-field-initializers
CFLAGS += -fno-omit-frame-pointer -fdiagnostics-show-option -fPIC
CFLAGS += -I$(REPY)/sdk/toolchain/linux_x86_glibc/nacl64/include
LDFLAGS += -L$(LIBS_PATH) -melf64_nacl -m64 -fPIC

all: | repy_check
	$(MAKE) com1.nexe com2.nexe
	@echo "Done."

liblogger.so: logger.o
	$(LD) $< $(LDFLAGS) -shared -Wl,-soname,liblogger.so.0 -o liblogger.so.0.0.1
	mkdir -p $(LIBS_PATH) $(RT_LIBS_PATH)
	cp liblogger.so.0.0.1 $(LIBS_PATH)/
	ln -sfv $(LIBS_PATH)/liblogger.so.0.0.1 $(LIBS_PATH)/liblogger.so.0
	ln -sfv $(LIBS_PATH)/liblogger.so.0.0.1 $(LIBS_PATH)/liblogger.so
	cp liblogger.so.0.0.1 $(RT_LIBS_PATH)/
	ln -sfv $(RT_LIBS_PATH)/liblogger.so.0.0.1 $(RT_LIBS_PATH)/liblogger.so.0
	ln -sfv $(RT_LIBS_PATH)/liblogger.so.0.0.1 $(RT_LIBS_PATH)/liblogger.so

com1.nexe: simple_tester.o sostub.o liblind.o
	$(LD) $^ $(LDFLAGS) -o $@

com2.nexe: liblogger.so | soserver.o liblind.o
	$(LD) $| $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

repy_check:
# sanity check
ifeq ($(strip $(REPY)),)
	$(error Need to set REPY_PATH)
endif
	@echo "Repy found, continuing."

clean:
	rm -rf *.o
	rm -rf *.nexe
	rm -f lind.log
	rm -f liblogger.so
	rm -f liblogger.so.0.0.1
	rm -f $(LIBS_PATH)/liblogger.so.0.0.1 $(LIBS_PATH)/liblogger.so.0
	rm -f $(LIBS_PATH)/liblogger.so $(RT_LIBS_PATH)/liblogger.so.0.0.1
	rm -f $(RT_LIBS_PATH)/liblogger.so.0 $(RT_LIBS_PATH)/liblogger.so
