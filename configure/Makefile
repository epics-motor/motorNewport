TOP=..

include $(TOP)/configure/CONFIG

ifeq ($(INSTALL_CONFIG),$(MOTOR)/configure)
# Don't overwrite Motor's configure files
INSTALL_CONFIG =
endif

TARGETS = $(CONFIG_TARGETS)

include $(TOP)/configure/RULES
