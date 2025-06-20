/*
FILENAME...  AGAP_CONEX.h
USAGE...      Motor driver support for the Newport CONEX-AGP and CONEX-CC series of controllers.

Mark Rivers
April 11, 2013

*/

#include "asynMotorController.h"
#include "asynMotorAxis.h"

typedef enum {
  ModelConexAGP,
  ModelConexCC,
  ModelConexPP,
  ModelDL,
  ModelFCL200
} ConexModel_t;

// No controller-specific parameters yet
#define NUM_AGAP_CONEX_PARAMS 0  

class epicsShareClass AGAP_CONEXAxis : public asynMotorAxis
{
public:
  /* These are the methods we override from the base class */
  AGAP_CONEXAxis(class AGAP_CONEXController *pC, int axisNumber);
  void report(FILE *fp, int level);
  asynStatus move(double position, int relative, double min_velocity, double max_velocity, double acceleration);
  asynStatus moveVelocity(double min_velocity, double max_velocity, double acceleration);
  asynStatus home(double min_velocity, double max_velocity, double acceleration, int forwards);
  asynStatus stop(double acceleration);
  asynStatus poll(bool *moving);
  asynStatus setPosition(double position);
  asynStatus setClosedLoop(bool closedLoop);
  asynStatus setPGain(double pGain);
  asynStatus setIGain(double iGain);
  asynStatus setDGain(double dGain);

private:
  AGAP_CONEXController *pC_;        /**< Pointer to the asynMotorController to which this axis belongs.
                                   *   Abbreviated because it is used very frequently */
  asynStatus getClosedLoop(bool *closedLoop);
  double currentPosition_;
  double encoderIncrement_;
  double interpolationFactor_;
  double stepSize_;
  double fullStepSize_;
  int    microStepsPerFullStep_;
  double highLimit_;
  double lowLimit_;
  double KP_;
  double KI_;
  double KD_;
  double LF_;
  double KPMax_;
  double KDMax_;
  double KIMax_;
  double LFMax_;
  char   stageID_[40];
  char   axisName_;
  int    axisNumber_;
  ConexModel_t conexModel_;
  
friend class AGAP_CONEXController;
};

class epicsShareClass AGAP_CONEXController : public asynMotorController {
public:
  AGAP_CONEXController(const char *portName, const char *serialPortName, int controllerID, double movingPollPeriod, double idlePollPeriod);

  void report(FILE *fp, int level);
  AGAP_CONEXAxis* getAxis(asynUser *pasynUser);
  AGAP_CONEXAxis* getAxis(int axisNo);
  asynStatus writeCONEX();
  asynStatus writeCONEX(const char *output, double timeout);

private:
  int controllerID_;
  char controllerVersion_[40];

  friend class AGAP_CONEXAxis;
};
