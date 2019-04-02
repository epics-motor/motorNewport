TOP=..

include $(TOP)/configure/CONFIG

TARGETS = $(CONFIG_TARGETS)
CONFIGS += $(subst ../,,$(wildcard $(CONFIG_INSTALLS)))

# Workaround to prevent overwriting motor's CONFIG_SITE
INSTALL_CONFIG = 

include $(TOP)/configure/RULES
