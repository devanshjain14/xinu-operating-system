include(${PROJECT_SOURCE_DIR}/compile/platform-cmake/arm-common.cmake)

#####################################################################
# Platform Specific Build Targets                                   #
#####################################################################


#####################################################################
# Platform Specific Installation Targets                            #
#####################################################################

install(FILES ${PROJECT_BINARY_DIR}/${XINU_BOOT_NAME}
  DESTINATION ${XINU_TFTP_ROOT})

#####################################################################
# Platform Specific Build Variables                                 #
#####################################################################

include(CMakeForceCompiler)

unset(XINU_PLATFORM CACHE)
set(XINU_PLATFORM "arm-bbb"
  CACHE STRING "Target platform" FORCE)

unset(XINU_PLATFORM_C_FLAGS CACHE)
set(XINU_PLATFORM_C_FLAGS "-mcpu=cortex-a8 -g"
  CACHE STRING "Platform specific compiler flags." FORCE)

unset(XINU_PLATFORM_CMAKE_FILE CACHE)
set(XINU_PLATFORM_CMAKE_FILE ${PROJECT_SOURCE_DIR}/compile/platform-cmake/arm-bbb.cmake
  CACHE STRING "Location of platform CMake file.")

#####################################################################
# Platform Specific Device Configuration                            #
#####################################################################

set(XINU_HAS_DEV_ETH        true )
set(XINU_HAS_DEV_LFS        true )
set(XINU_HAS_DEV_LOOPBACK   false)
set(XINU_HAS_DEV_NAM        true )
set(XINU_HAS_DEV_RAM        true )
set(XINU_HAS_DEV_RDS        true )
set(XINU_HAS_DEV_RFS        true )
set(XINU_HAS_DEV_TTY        true )
set(XINU_HAS_DEV_UART       false)
set(XINU_HAS_DEV_UART_PL011 false)
