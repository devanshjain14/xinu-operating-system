# Welcome to Xinu

# Build instructions

Copy the file compile/Makedefs.EXAMPLE to compile/Makedefs and make appropriate changes if necessary.  Make sure that the correct COMPILER_ROOT, LIBGCC_LOC and CONF_LFLAGS are set.

The PLATFORM variable should be set to one of:

- arm-qemu
- arm-bbb
- x86-qemu
- x86-galileo

