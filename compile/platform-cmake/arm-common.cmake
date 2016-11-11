#####################################################################
# Architecture Specific Toolchain: arm-none-eabi                    #
#####################################################################

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION ${XINU_VERSION_STRING})

unset(XINU_ARCH CACHE)
set(XINU_ARCH "arm"
  CACHE STRING "Target architecture" FORCE)

CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)

unset(XINU_ARCH_C_FLAGS CACHE)
set(XINU_ARCH_C_FLAGS "-marm -mno-unaligned-access"
  CACHE STRING "Architecture specific compiler flags." FORCE)

unset(XINU_LINK_FLAGS CACHE)
set(XINU_LINK_FLAGS -dn -m armelf -Map ${PROJECT_BINARY_DIR}/xinu.map -T ${PROJECT_SOURCE_DIR}/arch/ARM/ld.script
  CACHE STRING "Flags used to link the XINU binary." FORCE)

unset(XINU_MKIMAGE_FLAGS CACHE)
set(XINU_MKIMAGE_FLAGS -A arm -O linux -T kernel -C none -a 0x81000000 -e 0x81000000
  CACHE STRING "Flags used by the mkimage command." FORCE)
