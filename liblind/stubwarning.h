
#ifndef _STUB_WARNING_H_
#define _STUB_WARNING_H_

/*
 * A canned warning for sysdeps/stub functions.
 * The GNU linker prepends a "warning: " string.
 */
#define link_warning(symbol, msg)									\
 static const char __evoke_link_warning_ ## symbol[]							\
    __attribute__((__used__, section (".gnu.warning." #symbol "\n\t#"))) = msg


#define stub_warning(name) \
  do {													\
	  link_warning(name, "the `" #name "' function is not implemented and will always fail");	\
  } while (0)

int lind_fd(void);

#endif
