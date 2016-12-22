#include "SerialReader.h"

#define CONTROLLER_A    // ECAM/FMC, RADIO, GEAR
//#define CONTROLLER_B    // AP, Flight Controls
//#define CONTROLLER_C    // PFD


#ifdef CONTROLLER_A
  #include "ECAM.h"
  #include "Radio.h"
  #include "Gear.h"
  #include "Systems.h"
#endif


#ifdef CONTROLLER_B
  #include "AP.h"
  #include "ControlSurfaces.h"
#endif


#ifdef CONTROLLER_C
  #include "PFDConnector.h"
#endif
