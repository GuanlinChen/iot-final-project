/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2015. 
 * The use of this software is subject to the XDK SDK EULA
 */

/* module includes ********************************************************** */

/* system header files */
#include <stdint.h>
#include <stdio.h>
/* additional interface header files */
#include "OS_operatingSystem_ih.h"
#include "BCDS_Basics.h"
#include "OS_operatingSystem_ih.h"
#include "BCDS_Environmental.h"
#include "BCDS_LightSensor.h"
#include "XdkSensorHandle.h"
#include "BCDS_Assert.h"
#include "OS_operatingSystem_ph.h"
#include "OS_SleepMgt_ih.h"
/* own header files */
#include "XdkApplicationTemplate.h"

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */

/* global variables ********************************************************* */
static OS_timerHandle_tp printTimerHandle;
/* inline functions ********************************************************* */

/* local functions ********************************************************** */





/* global functions ********************************************************* */

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
void environmentalSensorDeinit(void)
{
    uint32_t returnValue;

    returnValue = Environmental_deInit(xdkEnvironmental_BME280_Handle);
    LightSensor_deInit(xdkLightSensor_MAX44009_Handle);
    if (returnValue == 0)
    	fprintf(stderr, "Deinit Success!\n");
    else
    	fprintf(stderr, "Deinit Error!\n");
}

void getEnvData(void *pvParameters)
{
	(void) pvParameters;
	uint16_t luxRawData = UINT16_C(0);
	Environmental_Data_T bme280 = { INT32_C(0), UINT32_C(0), UINT32_C(0) };
	Environmental_readData(xdkEnvironmental_BME280_Handle, &bme280);
	LightSensor_readRawData(xdkLightSensor_MAX44009_Handle,
		            &luxRawData);
	//printf("The light is: %x\n ", luxRawData);
	printf("{[MONITOR]%s(%llu): pressure=%ld%s | temperature=%ld%s | humidity=%ld%s | light=%d%s}\n",
			"BME280", OS_timeGetSystemTimeMs(),
			(long int)bme280.pressure,"pa",
			(long int)bme280.temperature,"mDeg",
			(long int)bme280.humidity,"\%rh",
			luxRawData,"lux");

}

void environmentalSensorInit()
{
	uint32_t returnValue;
	returnValue = Environmental_init(xdkEnvironmental_BME280_Handle);
	LightSensor_init(xdkLightSensor_MAX44009_Handle);
	printf("In the init function\n");
	if (returnValue == 0)
	{
		printf("Initialize success!\n");
		printTimerHandle = OS_timerCreate((const int8_t *) "getEnvData",
		        3000, 1, NULL, getEnvData);

		OS_timerStart(printTimerHandle, 0xffff);

	}

}




void init()
{
	 environmentalSensorInit();

}

void appInitSystem(OS_timerHandle_tp xTimer)
{
    (void) (xTimer);
    init();
}

extern void deinit(void)
{
    environmentalSensorDeinit();

}

/** ************************************************************************* */
