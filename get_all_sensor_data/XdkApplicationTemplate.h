/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2015. 
 * The use of this software is subject to the XDK SDK EULA
 */

/* header definition ******************************************************** */
#ifndef XDK110_XDKAPPLICATIONTEMPLATE_H_
#define XDK110_XDKAPPLICATIONTEMPLATE_H_

/* local interface declaration ********************************************** */
void environmentalSensorDeinit(void);
void getEnvData(void *pvParameters);
void environmentalSensorInit(void);
//void init();
void appInitSystem(OS_timerHandle_tp xTimer);
extern void deinit(void);
/* local type and macro definitions */

/* local function prototype declarations */

/* local module global variable declarations */

/* local inline function definitions */


#endif /* XDK110_XDKAPPLICATIONTEMPLATE_H_ */

/** ************************************************************************* */
