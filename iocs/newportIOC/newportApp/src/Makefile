TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

# The following are used for debugging messages.
#!USR_CXXFLAGS += -DDEBUG

#=============================
# Build the IOC application

PROD_IOC = newport
# newport.dbd will be created and installed
DBD += newport.dbd

# newport.dbd will be made up from these files:
newport_DBD += base.dbd

# Include dbd files from all support applications:
#ifdef ASYN
newport_DBD += asyn.dbd
newport_DBD += drvAsynSerialPort.dbd
#endif
newport_DBD += motorSupport.dbd
newport_DBD += devNewport.dbd

# Add all the support libraries needed by this IOC
newport_LIBS += Newport
newport_LIBS += motor
#ifdef ASYN
newport_LIBS += asyn
#endif
#ifdef SNCSEQ
newport_LIBS += seq pv
#endif

# newport_registerRecordDeviceDriver.cpp derives from newport.dbd
newport_SRCS += newport_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
newport_SRCS_DEFAULT += newportMain.cpp
newport_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#newport_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
newport_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

