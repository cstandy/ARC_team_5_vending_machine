# Application name
APPL ?= mbedtls_ssl_server2

EXT_DEV_LIST += wifi/mrf24g sensor/temperature/adt7420

# Heap Size Settings
HEAPSZ ?= 1048576
STACKSZ ?= 1048576

APPL_DEFINES  = -DNTCONF_EDITOR_MAXLEN=256 -DMBEDTLS_CONFIG_FILE=\"mbedtls_config.h\"

#
# root dir of embARC
#
EMBARC_ROOT = ../../../../../..

OS_SEL = freertos

MID_SEL = common ntshell mbedtls fatfs lwip lwip-contrib

# application source dirs
APPL_CSRC_DIR = .
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = .

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk
