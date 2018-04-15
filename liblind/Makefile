REPY != realpath $(REPY_PATH) 2>/dev/null || echo $(REPY_PATH)
CC = $(REPY)/sdk/linux_x86/bin/x86_64-nacl-gcc
LD = $(CC)
CFLAGS = -c -std=gnu99 -std=gnu99 -Wall -Wextra -Wno-long-long -fomit-frame-pointer -Wall -fdiagnostics-show-option -pedantic -m64 -D_GNU_SOURCE=1 -D__STDC_FORMAT_MACROS=1 -D_BSD_SOURCE=1 -D_POSIX_C_SOURCE=199506 -D_XOPEN_SOURCE=600 -Wno-unused-parameter
INCLUDE = -I$(REPY)/sdk/linux_x86/nacl64/include
RT_LIBS_PATH = $(REPY)lib/libs
LIBS_PATH = $(REPY)/sdk/linux_x86/nacl64/lib64
LDFLAGS = -L$(LIBS_PATH) -melf64_nacl -m64
LDLIBS = -llogger

all: installed_logger com1.nexe com2.nexe
	@echo "Done."

installed_logger: repy_check liblogger.so
	mkdir -p $(LIBS_PATH) $(RT_LIBS_PATH)
	cp liblogger.so.0.0.1 $(LIBS_PATH)/
	ln -sfv $(LIBS_PATH)/liblogger.so.0.0.1 $(LIBS_PATH)/liblogger.so.0
	ln -sfv $(LIBS_PATH)/liblogger.so.0.0.1 $(LIBS_PATH)/liblogger.so
	cp liblogger.so.0.0.1 $(RT_LIBS_PATH)
	ln -sfv $(RT_LIBS_PATH)/liblogger.so.0.0.1 $(RT_LIBS_PATH)/liblogger.so.0
	ln -sfv $(RT_LIBS_PATH)/liblogger.so.0.0.1 $(RT_LIBS_PATH)/liblogger.so

liblogger.so: logger.o
	$(CC) -shared -Wl,-soname,liblogger.so.0 -o liblogger.so.0.0.1 $<

com1.nexe: simple_tester.o sostub.o liblind.o
	$(LD) $(LDFLAGS) $? -o $@

com2.nexe: installed_logger soserver.o liblind.o
	$(LD) $(LDFLAGS) $(LDLIBS) soserver.o liblind.o -o $@

logger.o: %.o: logger.c
	$(CC) -fPIC $(CFLAGS) $(INCLUDE) $< -o $@

repy_check:
# sanity check
ifeq ($(strip $(REPY)),)
	$(error Need to set REPY_PATH)
endif
	@echo "Repy found, continuing."

clean:
	rm -rf *.o
	rm -rf *.nexe
	rm -f liblogger.so.0.0.1
	rm -f liblogger.so
	rm -f $(LIBS_PATH)/liblogger.so.0.0.1 $(LIBS_PATH)/liblogger.so.0 $(LIBS_PATH)/liblogger.so
	rm -f $(RT_LIBS_PATH)/liblogger.so.0.0.1 $(RT_LIBS_PATH)/liblogger.so.0 $(RT_LIBS_PATH)/liblogger.so
	rm -f lind.log
