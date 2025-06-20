/*
FILENAME... AGAP_CONEX.cpp
USAGE...    Motor driver support for the Newport CONEX-AGAP controllers

The AGAP uses a different syntax than the drivers in AGAP_CONEX, because it needs an axis identifier after more 2 letter commands.

Mark Rivers
June 19, 2025

*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <iocsh.h>
#include <epicsThread.h>

#include <asynOctetSyncIO.h>

#include "asynMotorController.h"
#include "asynMotorAxis.h"

#include <epicsExport.h>
#include "AGAP_CONEX.h"

#define NINT(f) (int)((f)>0 ? (f)+0.5 : (f)-0.5)

#define CONEX_TIMEOUT 2.0
#define LINUX_WRITE_DELAY 0.1

/** Creates a new AGAP_CONEXController object.
  * \param[in] portName          The name of the asyn port that will be created for this driver
  * \param[in] serialPortName    The name of the drvAsynSerialPort that was created previously to connect to the CONEX controller 
  * \param[in] controllerID      The controller ID
  * \param[in] movingPollPeriod  The time between polls when any axis is moving 
  * \param[in] idlePollPeriod    The time between polls when no axis is moving 
  */
AGAP_CONEXController::AGAP_CONEXController(const char *portName, const char *serialPortName, int controllerID, 
                                           double movingPollPeriod, double idlePollPeriod)
  :  asynMotorController(portName, 2, NUM_AGAP_CONEX_PARAMS, 
                         0, // No additional interfaces beyond those in base class
                         0, // No additional callback interfaces beyond those in base class
                         ASYN_CANBLOCK | ASYN_MULTIDEVICE, 
                         1, // autoconnect
                         0, 0),  // Default priority and stack size
   controllerID_(controllerID)
                    
{
  asynStatus status;
  static const char *functionName = "AGAP_CONEXController::AGAP_CONEXController";

  /* Connect to CONEX controller */
  status = pasynOctetSyncIO->connect(serialPortName, 0, &pasynUserController_, NULL);
  if (status) {
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, 
      "%s: cannot connect to CONEX controller\n",
      functionName);
    return;
  }
  
  // Flush any characters that controller has, read firmware version
  sprintf(outString_, "%dVE", controllerID_);
  status = writeReadController();
  if (status) {
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, 
      "%s: cannot read version information from AGAP_CONEX controller\n",
      functionName);
    return;
  }
  strcpy(controllerVersion_, &inString_[4]);

  // Create the axis objects
  for (int axis=0; axis<2; axis++) {
    new AGAP_CONEXAxis(this, axis);
  }
  startPoller(movingPollPeriod, idlePollPeriod, 2);
}


/** Creates a new AGAP_CONEXController object.
  * Configuration command, called directly or from iocsh
  * \param[in] portName          The name of the asyn port that will be created for this driver
  * \param[in] serialPortName    The name of the drvAsynSerialPort that was created previously to connect to the CONEX controller 
  * \param[in] controllerID      The controller ID
  * \param[in] movingPollPeriod  The time between polls when any axis is moving 
  * \param[in] idlePollPeriod    The time between polls when no axis is moving 
  */
extern "C" {

int AGAP_CONEXCreateController(const char *portName, const char *serialPortName, int controllerID, 
                               int movingPollPeriod, int idlePollPeriod)
{
  new AGAP_CONEXController(portName, serialPortName, controllerID, movingPollPeriod/1000., idlePollPeriod/1000.);
  return(asynSuccess);
}

} // extern "C" 


/** Writes a string to the controller.
  * Calls writeCONEX() with a default location of the string to write and a default timeout. */ 
asynStatus AGAP_CONEXController::writeCONEX()
{
  return writeCONEX(outString_, CONEX_TIMEOUT);
}

/** Writes a string to the controller.
  * \param[in] output The string to be written.
  * \param[in] timeout Timeout before returning an error.*/
asynStatus AGAP_CONEXController::writeCONEX(const char *output, double timeout)
{
  size_t nwrite;
  asynStatus status;
  // const char *functionName="writeCONEX";
  
  status = pasynOctetSyncIO->write(pasynUserController_, output,
                                   strlen(output), timeout, &nwrite);
                                   
  // On Linux it seems to be necessary to delay a short time between writes
  #ifdef linux
  epicsThreadSleep(LINUX_WRITE_DELAY);
  #endif
                                  
  return status ;
}

/** Reports on status of the driver
  * \param[in] fp The file pointer on which report information will be written
  * \param[in] level The level of report detail desired
  *
  * If details > 0 then information is printed about each axis.
  * After printing controller-specific information it calls asynMotorController::report()
  */
void AGAP_CONEXController::report(FILE *fp, int level)
{
  fprintf(fp, "CONEX motor driver %s, controllerID=%d, version=\"%s\n"
              "  moving poll period=%f, idle poll period=%f\n", 
    this->portName, controllerID_, controllerVersion_, movingPollPeriod_, idlePollPeriod_);

  // Call the base class method
  asynMotorController::report(fp, level);
}

/** Returns a pointer to an AGAP_CONEXAxis object.
  * Returns NULL if the axis number encoded in pasynUser is invalid.
  * \param[in] pasynUser asynUser structure that encodes the axis index number. */
AGAP_CONEXAxis* AGAP_CONEXController::getAxis(asynUser *pasynUser)
{
  return static_cast<AGAP_CONEXAxis*>(asynMotorController::getAxis(pasynUser));
}

/** Returns a pointer to an AGAP_CONEXAxis object.
  * Returns NULL if the axis number encoded in pasynUser is invalid.
  * \param[in] axisNo Axis index number. */
AGAP_CONEXAxis* AGAP_CONEXController::getAxis(int axisNo)
{
  return static_cast<AGAP_CONEXAxis*>(asynMotorController::getAxis(axisNo));
}


// These are the AGAP_CONEXAxis methods

/** Creates a new AGAP_CONEXAxis object.
  * \param[in] pC Pointer to the AGAP_CONEXController to which this axis belongs. 
  * \param[in] axisNo Index number of this axis, range 0 to pC->numAxes_-1.
  * 
  * Initializes register numbers, etc.
  */
AGAP_CONEXAxis::AGAP_CONEXAxis(AGAP_CONEXController *pC, int axisNumber)
  : asynMotorAxis(pC, axisNumber),
    pC_(pC), 
    currentPosition_(0.),
    axisNumber_(axisNumber)
{
  static const char *functionName = "AGAP_CONEXAxis::AGAP_CONEXAxis";

  if (axisNumber == 0) {
    axisName_ = 'U';
  } else {
    axisName_ = 'V';
  }
  
  // avoid constructor warnings
  KD_=0.;
  KI_=0.;
  LF_=0.;
  KP_=0.;

  // Figure out what model this is
  if (strstr(pC->controllerVersion_, "CONEX-AGAP")) {
    KPMax_ = 31.;
    KIMax_ = 31.;
    LFMax_ = 31.;
  } 
  else {
    asynPrint(pC->pasynUserSelf, ASYN_TRACE_ERROR,
      "%s: unknown model, firmware string=%s\n",
      functionName, pC->controllerVersion_);
    return;
  }

  // Read the stage ID
  sprintf(pC_->outString_, "%dID?", pC->controllerID_);
  pC_->writeReadController();
  strcpy(stageID_, &pC_->inString_[3]);

  // Read the system resolution)
  sprintf(pC_->outString_, "%dSU?", pC->controllerID_);
  pC_->writeReadController();
  encoderIncrement_ = atof(&pC_->inString_[3]);

  // AGAP operates in mm and doubles, no conversion required
  // but MotorRecord expects this value to be integer, to prevent
  // decimals to be stripped off by MotorRecord, we multiply by 1e4 
  // and then set resolution to 1e-4
  stepSize_ = 0.0001;
  
  // Read the low and high software limits
  sprintf(pC_->outString_, "%dSL%c?", pC->controllerID_, axisName_);
  pC_->writeReadController();
  lowLimit_ = atof(&pC_->inString_[4]);
  sprintf(pC_->outString_, "%dSR%c?", pC->controllerID_, axisName_);
  pC_->writeReadController();
  highLimit_ = atof(&pC_->inString_[4]);  
  
}

/** Reports on status of the axis
  * \param[in] fp The file pointer on which report information will be written
  * \param[in] level The level of report detail desired
  *
  * After printing device-specific information calls asynMotorAxis::report()
  */
void AGAP_CONEXAxis::report(FILE *fp, int level)
{
  if (level > 0) {
    sprintf(pC_->outString_, "%dKP%c?", pC_->controllerID_, axisName_);
    pC_->writeReadController();
    KP_ = atof(&pC_->inString_[4]);
    sprintf(pC_->outString_, "%dKI%c?", pC_->controllerID_, axisName_);
    pC_->writeReadController();
    KI_ = atof(&pC_->inString_[4]);
    sprintf(pC_->outString_, "%dLF%c?", pC_->controllerID_, axisName_);
    pC_->writeReadController();
    LF_ = atof(&pC_->inString_[4]);
    fprintf(fp, "axisName=%c\n"
                "  stageID=%s\n"
                "  currentPosition=%f, encoderIncrement=%f\n"
                "  interpolationFactor=%f, stepSize=%f, lowLimit=%f, highLimit=%f\n"
                "  KP=%f, KI=%f, KD/LF=%f\n",
            axisName_, stageID_,
            currentPosition_, encoderIncrement_, 
            interpolationFactor_, stepSize_, lowLimit_, highLimit_,
            KP_, KI_, LF_);
  }

  // Call the base class method
  asynMotorAxis::report(fp, level);
}

asynStatus AGAP_CONEXAxis::move(double position, int relative, double minVelocity, double maxVelocity, double acceleration)
{
  asynStatus status;
  // static const char *functionName = "AGAP_CONEXAxis::move";

  if (relative) {
    sprintf(pC_->outString_, "%dPR%c%f", pC_->controllerID_, axisName_, position*stepSize_);
  } else {
    sprintf(pC_->outString_, "%dPA%c%f", pC_->controllerID_, axisName_, position*stepSize_);
  }
  status = pC_->writeCONEX();
  return status;
}

asynStatus AGAP_CONEXAxis::home(double minVelocity, double maxVelocity, double acceleration, int forwards)
{
  asynStatus status = asynSuccess;
  //static const char *functionName = "AGAP_CONEXAxis::home";

  return status;
}

asynStatus AGAP_CONEXAxis::moveVelocity(double minVelocity, double maxVelocity, double acceleration)
{
  asynStatus status;
  //static const char *functionName = "AGAP_CONEXAxis::moveVelocity";

  sprintf(pC_->outString_, "%dJA%c%f", pC_->controllerID_, axisName_, maxVelocity);
  status = pC_->writeCONEX();
  return status;
}

asynStatus AGAP_CONEXAxis::stop(double acceleration )
{
  asynStatus status;
  //static const char *functionName = "AGAP_CONEXAxis::stop";

  sprintf(pC_->outString_, "%dST%c", pC_->controllerID_, axisName_);
  status = pC_->writeCONEX();
  return status;
}

asynStatus AGAP_CONEXAxis::setPosition(double position)
{
  //static const char *functionName = "AGAP_CONEXAxis::setPosition";

  return asynSuccess;
}

asynStatus AGAP_CONEXAxis::setClosedLoop(bool closedLoop)
{
  asynStatus status;
  //static const char *functionName = "AGAP_CONEXAxis::setClosedLoop";

  sprintf(pC_->outString_, "%dMM%d", pC_->controllerID_, closedLoop ? 1 : 0);
  status = pC_->writeCONEX();
  return status;
}

asynStatus AGAP_CONEXAxis::getClosedLoop(bool *closedLoop)
{
  int status;
  asynStatus comStatus;
  
    // Read the status of the motor
  sprintf(pC_->outString_, "%dMM?", pC_->controllerID_);
  comStatus = pC_->writeReadController();
  // The response string is of the form "1MMn"
  sscanf(pC_->inString_, "%*dMM%x", &status);
  *closedLoop = (status >= 0x28) && (status <= 0x36);
  return comStatus;
}

asynStatus AGAP_CONEXAxis::setPGain(double pGain)
{
  asynStatus status = asynSuccess;
  bool closedLoop;
  //static const char *functionName = "AGAP_CONEXAxis::setPGain";

  getClosedLoop(&closedLoop);
  setClosedLoop(false);
  // The pGain value from the motor record is between 0 and 1.
  sprintf(pC_->outString_, "%dKP%c%f", pC_->controllerID_, axisName_, pGain*KPMax_);
  status = pC_->writeCONEX();
  if (closedLoop) setClosedLoop(true);

  return status;
}

asynStatus AGAP_CONEXAxis::setIGain(double iGain)
{
  asynStatus status = asynSuccess;
  bool closedLoop;
  //static const char *functionName = "AGAP_CONEXAxis::setIGain";

  getClosedLoop(&closedLoop);
  setClosedLoop(false);
  // The iGain value from the motor record is between 0 and 1.
  sprintf(pC_->outString_, "%dKI%c%f", pC_->controllerID_, axisName_, iGain*KIMax_);
  status = pC_->writeCONEX();
  if (closedLoop) setClosedLoop(true);
  return status;
}

asynStatus AGAP_CONEXAxis::setDGain(double dGain)
{
  asynStatus status = asynSuccess;
  //static const char *functionName = "AGAP_CONEXAxis::setDGain";

  // We are using the DGain for the Low pass filter frequency.
  // DGain value is between 0 and 1
  sprintf(pC_->outString_, "%dLF%f", pC_->controllerID_, dGain*LFMax_);
  status = pC_->writeCONEX();
  return status;
}

/** Polls the axis.
  * This function reads the motor position, the limit status, the home status, the moving status, 
  * and the drive power-on status. 
  * It calls setIntegerParam() and setDoubleParam() for each item that it polls,
  * and then calls callParamCallbacks() at the end.
  * \param[out] moving A flag that is set indicating that the axis is moving (true) or done (false). */
asynStatus AGAP_CONEXAxis::poll(bool *moving)
{ 
  int done=1;
  double position;
  unsigned int status;
  unsigned int state;
  int count;
  bool closedLoop;
  asynStatus comStatus;

  // Read the current motor position
  sprintf(pC_->outString_, "%dTP%c", pC_->controllerID_, axisName_);
  comStatus = pC_->writeReadController();
  if (comStatus) goto skip;
  // The response string is of the form "1TPUxxx"
  position = atof(&pC_->inString_[4]);
  currentPosition_ = position /stepSize_;
  setDoubleParam(pC_->motorPosition_, currentPosition_);

  // Read the moving status of this motor
  sprintf(pC_->outString_, "%dTS", pC_->controllerID_);
  comStatus = pC_->writeReadController();
  if (comStatus) goto skip;
  
  // The response string is of the form "1TSabcdef"
  count = sscanf(pC_->inString_, "%*dTS%*4c%x", &status);
  if (count != 1) goto skip;

  state = status & 0xff;
  if ((state == 0x28) || (state == 0x29) || state == 0x46) done = 0;

  setIntegerParam(pC_->motorStatusDone_, done);
  *moving = done ? false:true;

  setIntegerParam(pC_->motorStatusLowLimit_, 0);
  setIntegerParam(pC_->motorStatusHighLimit_, 0);
  
  // Set the power-on (closed loop) status of the motor
  comStatus = getClosedLoop(&closedLoop);
  if (comStatus) goto skip;
  setIntegerParam(pC_->motorStatusPowerOn_, closedLoop ? 1:0);

  skip:
  setIntegerParam(pC_->motorStatusProblem_, comStatus ? 1:0);
  callParamCallbacks();
  return comStatus ? asynError : asynSuccess;
}

/** Code for iocsh registration */
static const iocshArg AGAP_CONEXCreateControllerArg0 = {"Port name", iocshArgString};
static const iocshArg AGAP_CONEXCreateControllerArg1 = {"Serial port name", iocshArgString};
static const iocshArg AGAP_CONEXCreateControllerArg2 = {"Controller ID", iocshArgInt};
static const iocshArg AGAP_CONEXCreateControllerArg3 = {"Moving poll period (ms)", iocshArgInt};
static const iocshArg AGAP_CONEXCreateControllerArg4 = {"Idle poll period (ms)", iocshArgInt};
static const iocshArg * const AGAP_CONEXCreateControllerArgs[] = {&AGAP_CONEXCreateControllerArg0,
                                                                &AGAP_CONEXCreateControllerArg1,
                                                                &AGAP_CONEXCreateControllerArg2,
                                                                &AGAP_CONEXCreateControllerArg3,
                                                                &AGAP_CONEXCreateControllerArg4};
static const iocshFuncDef AGAP_CONEXCreateControllerDef = {"AGAP_CONEXCreateController", 5, AGAP_CONEXCreateControllerArgs};
static void AGAP_CONEXCreateContollerCallFunc(const iocshArgBuf *args)
{
  AGAP_CONEXCreateController(args[0].sval, args[1].sval, args[2].ival, args[3].ival, args[4].ival);
}

static void AGAP_CONEXRegister(void)
{
  iocshRegister(&AGAP_CONEXCreateControllerDef, AGAP_CONEXCreateContollerCallFunc);
}

extern "C" {
epicsExportRegistrar(AGAP_CONEXRegister);
}
