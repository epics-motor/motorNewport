TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += HXP_coords.db
DB += HXP_extra.db
DB += profileMoveAxisXPS.template
DB += profileMoveControllerXPS.template
DB += profileMoveControllerXPS_settings.req
DB += XPSAuxAi.db
DB += XPSAuxAo.db
DB += XPSAuxBi.db
DB += XPSAuxBo.db
DB += XPSAuxLi.db
DB += XPSAuxLo.db
DB += XPS_extra.db
DB += XPSPositionCompare.db
DB += XPSPositionCompare_settings.req
DB += xpsSlave.db
DB += XPSTclScript.template

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

ifeq ($(BASE_3_14),YES)
# Only needed for 3.14 build rules:
vpath %.req $(USR_VPATH) $(GENERIC_SRC_DIRS)
endif
