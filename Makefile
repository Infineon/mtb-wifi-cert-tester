################################################################################
# \file Makefile
# \version 1.0
#
# \brief
# Top-level appication make file.
#
################################################################################
# \copyright
# Copyright 2021 Cypress Semiconductor Corporation (an Infineon company)
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################


################################################################################
# Basic Configuration
################################################################################

# Target board/hardware
TARGET=CY8CKIT-062S2-43012
#TARGET=CY8CEVAL-062S2-LAI-4373M2
#TARGET=CY8CEVAL-062S2-MUR-43439M2
#TARGET=CY8CPROTO-062-4343W

# Name of application (used to derive name of final linked file).
APPNAME=mtb-wifi-cert-tester

# Name of toolchain to use. Options include:
#
# GCC_ARM -- GCC 9.3.1, provided with ModusToolbox IDE
# ARM     -- ARM Compiler (must be installed separately)
# IAR     -- IAR Compiler (must be installed separately)
#
# See also: CY_COMPILER_PATH below
TOOLCHAIN=GCC_ARM

# If set to "true" or "1", display full command-lines when building.
VERBOSE=


################################################################################
# Advanced Configuration
################################################################################

# Enable optional code that is ordinarily disabled by default.
#
# Available components depend on the specific targeted hardware and firmware
# in use. In general, if you have
#
#    COMPONENTS=foo bar
#
# ... then code in directories named COMPONENT_foo and COMPONENT_bar will be 
# added to the build
#
COMPONENTS=FREERTOS LWIP MBEDTLS WCM SECURE_SOCKETS PSOC6HAL

# Like COMPONENTS, but disable optional code that was enabled by default.
DISABLE_COMPONENTS=HCI-UART

# By default the build system automatically looks in the Makefile's directory
# tree for source code and builds it. The SOURCES variable can be used to
# manually add source code to the build process from a location not searched
# by default, or otherwise not found by the build system.
SOURCES=

# Like SOURCES, but for include directories. Value should be paths to
# directories (without a leading -I).
INCLUDES=./configs

MBEDTLSFLAGS = MBEDTLS_USER_CONFIG_FILE='"configs/mbedtls_user_config.h"'

# Add additional defines to the build process (without a leading -D).
DEFINES=$(MBEDTLSFLAGS) CYBSP_WIFI_CAPABLE CY_RTOS_AWARE CY_RETARGET_IO_CONVERT_LF_TO_CRLF TARGET_NAME=$(TARGET)

MTB_TYPE=COMBINED

# Build timestamp
BUILD_TIME := "Timestamp_$(shell date +%Y-%m-%dT%H:%M:%S%z)"
DEFINES+=BUILD_TIME_STAMP=$(BUILD_TIME)

# WiFi Cert Tester Version
file := version.xml
WIFI_CERT_VERSION_STRING :=  $(shell sed -e 's/<version>//g' -e 's/<\/version>//g'  ${file})
DEFINES+=WIFI_CERT_VER=$(WIFI_CERT_VERSION_STRING)

# Microsoft supplicant Client, RootCA Certificates and Private Key
#DEFINES+=MICROSOFT_SUPPLICANT_SERVER

# Disable WHD logging
DEFINES+=WHD_PRINT_DISABLE

#
# Define country to US  this will be set when
# WHD driver is initialized
DEFINES+=CY_WIFI_COUNTRY=WHD_COUNTRY_UNITED_STATES

# Select softfp or hardfp floating point. Default is softfp. 
VFP_SELECT=

# Additional / custom C compiler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
CFLAGS=

# Additional / custom C++ compiler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
CXXFLAGS=

# Additional / custom assembler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
ASFLAGS=

# Additional / custom linker flags.
LDFLAGS=

# Additional / custom libraries to link in to the application.
LDLIBS=

# Path to the linker script to use (if empty, use the default linker script).
LINKER_SCRIPT=

# Custom pre-build commands to run.
PREBUILD=

# Custom post-build commands to run.
POSTBUILD=

CY_LIBS_SEARCH_DEPTH=7

#
# This is setup to build with either the IAR compiler or the
# GCC compiler.  There is currently no support for the ARM
# compiler.
#
ifeq ($(TOOLCHAIN),IAR)
CY_IGNORE+=./libs/freertos/Source/portable/TOOLCHAIN_GCC_ARM
else
CY_IGNORE+=./libs/freertos/Source/portable/TOOLCHAIN_IAR
endif

#  Target specific flag for SDIO UHS supported mode
#  this flag also includes optimization such as CPU Clock @150Mhz
#  optimized word (32 bit) memcpy and release build
#
ifeq ($(TARGET), CY8CEVAL-062S2-LAI-4373M2)
DEFINES += SDIO_UHS_OPTIMIZATION
# Default build configuration. Options include:
#
# Debug   -- build with minimal optimizations, focus on debugging.
# Release -- build with full optimizations
CONFIG=Release
else
ifeq ($(TARGET), CY8CKIT-062S2-43012)
# Default build configuration. Options include:
#
# Debug   -- build with minimal optimizations, focus on debugging.
# Release -- build with full optimizations
CONFIG=Release
#DEFINES+= WPA3_UNIT_TEST_ENABLED
else
ifeq ($(TARGET), CY8CEVAL-062S2-MUR-43439M2)
# Default build configuration. Options include:
#
# Debug   -- build with minimal optimizations, focus on debugging.
# Release -- build with full optimizations
CONFIG=Release
else
# Default build configuration. Options include:
#
# Debug   -- build with minimal optimizations, focus on debugging.
# Release -- build with full optimizations
CONFIG=Debug
endif
endif
endif

################################################################################
# Paths
################################################################################

# Relative path to the project directory (default is the Makefile's directory).
#
# This controls where automatic source code discovery looks for code.
CY_APP_PATH=

# Relative path to the "base" library. It provides the core makefile build 
# infrastructure.

# Absolute path to the compiler's "bin" directory. 
#
# The default depends on the selected TOOLCHAIN (GCC_ARM uses the ModusToolbox
# IDE provided compiler by default).
CY_COMPILER_PATH ?=

CY_GETLIBS_SHARED_PATH=../
CY_GETLIBS_SHARED_NAME=mtb_shared

# Locate ModusToolbox IDE helper tools folders in default installation
# locations for Windows, Linux, and macOS.
CY_WIN_HOME=$(subst \,/,$(USERPROFILE))
CY_TOOLS_PATHS ?= $(wildcard \
    $(CY_WIN_HOME)/ModusToolbox/tools_* \
    $(HOME)/ModusToolbox/tools_* \
    /Applications/ModusToolbox/tools_*)

# If you install ModusToolbox IDE in a custom location, add the path to its
# "tools_X.Y" folder (where X and Y are the version number of the tools
# folder).
CY_TOOLS_PATHS+=

# Default to the newest installed tools folder, or the users override (if it's
# found).
CY_TOOLS_DIR=$(lastword $(sort $(wildcard $(CY_TOOLS_PATHS))))

ifeq ($(CY_TOOLS_DIR),)
$(error Unable to find any of the available CY_TOOLS_PATHS -- $(CY_TOOLS_PATHS))
endif

$(info Tools Directory: $(CY_TOOLS_DIR))

include $(CY_TOOLS_DIR)/make/start.mk
