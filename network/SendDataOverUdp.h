/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2015. 
 * The use of this software is subject to the XDK SDK EULA
 */

/* header definition ******************************************************** */
#ifndef XDK110_SENDDATAOVERUDP_H_
#define XDK110_SENDDATAOVERUDP_H_

/* local interface declaration ********************************************** */

/* local type and macro definitions */
typedef enum returnTypes_e
{
    STATUS_NOT_OK,
    STATUS_OK,
    SOCKET_ERROR,
    SEND_ERROR
} returnTypes_t;

#define ONESECONDDELAY UINT32_C(1000) 	            /**< Macro to represent one second time unit*/
#define FIVESECONDDELAY UINT32_C(5000)              /**< Macro to represent five second time unit*/

#define TIMERBLOCKTIME UINT32_MAX             /**< Macro used to define block time of a timer*/
#define ZERO    UINT8_C(0)  			            /**< Macro to define value zero*/

//#warning Please provide WLAN related configurations, with valid SSID & WPA key and server ip address where packets are to be sent in the below macros.
/** Network configurations */
#define WLAN_CONNECT_WPA_SSID                "FiOS-N7SSP"         /**< Macros to define WPA/WPA2 network settings */
#define WLAN_CONNECT_WPA_PASS                " "      /**< Macros to define WPA/WPA2 network settings */
#define BUFFER_SIZE        UINT8_C(4)
/** IP addressed of server side socket.Should be in long format, E.g: 0xc0a82177 == 192.168.33.119 */
#define SERVER_IP         UINT32_C(0xC0A801CC)
#define SERVER_PORT        UINT16_C(4119)           /**< Port number on which server will listen */

/* local function prototype declarations */
/**
 *  @brief
 *      Function to connect to wifi network and obtain IP address
 *
 *  @param [in ] xTimer
 */
static void wifiConnectGetIP(OS_timerHandle_tp xTimer);
/**
 *  @brief
 *      Function to periodically send data over WiFi as UDP packets. This is run by an Auto-reloading timer.
 *
 *  @param [in ] xTimer
 */
static void wifiSend(OS_timerHandle_tp xTimer);

/* local module global variable declarations */

/* local inline function definitions */

#endif /* XDK110_SENDDATAOVERUDP_H_ */

/** ************************************************************************* */
