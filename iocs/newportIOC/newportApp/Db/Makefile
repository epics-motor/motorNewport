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

### These databases come from motor
DB_INSTALLS += $(MOTOR)/db/asyn_motor.db
DB_INSTALLS += $(MOTOR)/db/asyn_motor_positions.req
DB_INSTALLS += $(MOTOR)/db/asyn_motor_settings.req
DB_INSTALLS += $(MOTOR)/db/basic_asyn_motor.db
DB_INSTALLS += $(MOTOR)/db/profileMoveController.template
DB_INSTALLS += $(MOTOR)/db/profileMoveController_settings.req
DB_INSTALLS += $(MOTOR)/db/profileMoveAxis.template
DB_INSTALLS += $(MOTOR)/db/profileMoveAxis_settings.req

ifdef MOTOR_NEWPORT
### motorNewport was built outside motor
DB_INSTALLS += $(MOTOR_NEWPORT)/db/HXP_extra.db
DB_INSTALLS += $(MOTOR_NEWPORT)/db/HXP_coords.db
DB_INSTALLS += $(MOTOR_NEWPORT)/db/profileMoveControllerXPS.template
DB_INSTALLS += $(MOTOR_NEWPORT)/db/profileMoveControllerXPS_settings.req
DB_INSTALLS += $(MOTOR_NEWPORT)/db/profileMoveAxisXPS.template
DB_INSTALLS += $(MOTOR_NEWPORT)/db/XPS_extra.db
DB_INSTALLS += $(MOTOR_NEWPORT)/db/XPSAuxBi.db
DB_INSTALLS += $(MOTOR_NEWPORT)/db/XPSAuxBo.db
DB_INSTALLS += $(MOTOR_NEWPORT)/db/XPSAuxLi.db
DB_INSTALLS += $(MOTOR_NEWPORT)/db/XPSAuxLo.db
DB_INSTALLS += $(MOTOR_NEWPORT)/db/XPSAuxAi.db
DB_INSTALLS += $(MOTOR_NEWPORT)/db/XPSAuxAo.db
else
### motorNewport was built inside motor/modules
DB_INSTALLS += $(MOTOR)/db/HXP_extra.db
DB_INSTALLS += $(MOTOR)/db/HXP_coords.db
DB_INSTALLS += $(MOTOR)/db/profileMoveControllerXPS.template
DB_INSTALLS += $(MOTOR)/db/profileMoveControllerXPS_settings.req
DB_INSTALLS += $(MOTOR)/db/profileMoveAxisXPS.template
DB_INSTALLS += $(MOTOR)/db/XPS_extra.db
DB_INSTALLS += $(MOTOR)/db/XPSAuxBi.db
DB_INSTALLS += $(MOTOR)/db/XPSAuxBo.db
DB_INSTALLS += $(MOTOR)/db/XPSAuxLi.db
DB_INSTALLS += $(MOTOR)/db/XPSAuxLo.db
DB_INSTALLS += $(MOTOR)/db/XPSAuxAi.db
DB_INSTALLS += $(MOTOR)/db/XPSAuxAo.db
endif

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

