include(${PROJECT_SOURCE_DIR}/compile/platform-cmake/arm-common.cmake)

#####################################################################
# Platform Specific Build Targets                                   #
#####################################################################

find_program(QEMU_BINARY qemu-system-arm)

unset(QEMU_DEBUG_PORT CACHE)
set(QEMU_DEBUG_PORT "5560"
    CACHE STRING "TCP port to use when connecting to QEMU's GDB interface.")

unset(QEMU_FLAGS CACHE)
set(QEMU_FLAGS -M versatilepb -m 512M -nographic -cpu arm1176
  CACHE STRING "Command line arguments for QEMU.")

unset(QEMU_ENV CACHE)
set(QEMU_ENV export QEMU_AUDIO_DRV=wav; export QEMU_WAV_PATH=$(PROJECT_SOURCE_DIR)/compile/.wav.wav;)

# QEMU boot command

add_custom_target(qemu
  ${QEMU_ENV} ${QEMU_BINARY} ${QEMU_FLAGS} -kernel ${PROJECT_BINARY_DIR}/${XINU_BOOT_NAME}
  DEPENDS ${XINU_BOOT_NAME}
  VERBATIM
  USES_TERMINAL)

# QEMU boot command w/ GDB

add_custom_target(qemu-gdb
  ${QEMU_ENV} ${QEMU_BINARY} ${QEMU_FLAGS} -kernel ${PROJECT_BINARY_DIR}/${XINU_BOOT_NAME} -S -gdb tcp::${QEMU_DEBUG_PORT}
  DEPENDS ${XINU_BOOT_NAME}
  VERBATIM
  USES_TERMINAL)

#####################################################################
# Platform Specific Installation Targets                            #
#####################################################################

#####################################################################
# Platform Specific Build Variables                                 #
#####################################################################

include(CMakeForceCompiler)

unset(XINU_PLATFORM CACHE)
set(XINU_PLATFORM "arm-qemu"
  CACHE STRING "Target platform" FORCE)

unset(XINU_PLATFORM_C_FLAGS CACHE)
set(XINU_PLATFORM_C_FLAGS "-mcpu=arm1176jz-s -ggdb3"
  CACHE STRING "Platform specific compiler flags." FORCE)
	
unset(XINU_PLATFORM_CMAKE_FILE CACHE)
set(XINU_PLATFORM_CMAKE_FILE ${PROJECT_SOURCE_DIR}/compile/platform-cmake/arm-qemu.cmake
  CACHE STRING "Location of platform CMake file.")

#####################################################################
# Platform Specific Device Configuration                            #
#####################################################################

set(XINU_HAS_DEV_ETH        false)
set(XINU_HAS_DEV_LFS        true )
set(XINU_HAS_DEV_LOOPBACK   true )
set(XINU_HAS_DEV_NAM        true )
set(XINU_HAS_DEV_RAM        true )
set(XINU_HAS_DEV_RDS        false)
set(XINU_HAS_DEV_RFS        false)
set(XINU_HAS_DEV_TTY        true )
set(XINU_HAS_DEV_UART       true )
set(XINU_HAS_DEV_UART_PL011 true )
