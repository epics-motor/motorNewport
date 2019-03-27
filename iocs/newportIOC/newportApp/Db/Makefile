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

###### NOTE: *.template and *.req fail to install properly

### These databases come from motor
DB += $(MOTOR)/db/asyn_motor.db
#!DB += $(MOTOR)/db/asyn_motor_positions.req
#!DB += $(MOTOR)/db/asyn_motor_settings.req
DB += $(MOTOR)/db/basic_asyn_motor.db
#!DB += $(MOTOR)/db/profileMoveController.template
#!DB += $(MOTOR)/db/profileMoveController_settings.req
#!DB += $(MOTOR)/db/profileMoveAxis.template
#!DB += $(MOTOR)/db/profileMoveAxis_settings.req

### These databases come from motorNewport and will reside in 
### motor if motorNewport was built inside motor/modules
DB += $(MOTOR)/db/HXP_extra.db
DB += $(MOTOR)/db/HXP_coords.db
#!DB += $(MOTOR)/db/profileMoveControllerXPS.template
#!DB += $(MOTOR)/db/profileMoveControllerXPS_settings.req
#!DB += $(MOTOR)/db/profileMoveAxisXPS.template
DB += $(MOTOR)/db/XPS_extra.db
DB += $(MOTOR)/db/XPSAuxBi.db
DB += $(MOTOR)/db/XPSAuxBo.db
DB += $(MOTOR)/db/XPSAuxLi.db
DB += $(MOTOR)/db/XPSAuxLo.db
DB += $(MOTOR)/db/XPSAuxAi.db
DB += $(MOTOR)/db/XPSAuxAo.db
### And they will reside in motorNewport if motorNewport was built
### outside motor
#DB += $(MOTOR_NEWPORT)/db/HXP_extra.db
#DB += $(MOTOR_NEWPORT)/db/HXP_coords.db
#DB += $(MOTOR_NEWPORT)/db/profileMoveControllerXPS.template
#DB += $(MOTOR_NEWPORT)/db/profileMoveControllerXPS_settings.req
#DB += $(MOTOR_NEWPORT)/db/profileMoveAxisXPS.template
#DB += $(MOTOR_NEWPORT)/db/XPS_extra.db
#DB += $(MOTOR_NEWPORT)/db/XPSAuxBi.db
#DB += $(MOTOR_NEWPORT)/db/XPSAuxBo.db
#DB += $(MOTOR_NEWPORT)/db/XPSAuxLi.db
#DB += $(MOTOR_NEWPORT)/db/XPSAuxLo.db
#DB += $(MOTOR_NEWPORT)/db/XPSAuxAi.db
#DB += $(MOTOR_NEWPORT)/db/XPSAuxAo.db

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

