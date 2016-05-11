/**
 * This software is copyrighted by Bosch Connected Devices and Solutions GmbH, 2015. 
 * The use of this software is subject to the XDK SDK EULA
 */

/* module includes ********************************************************** */

/* system header files */
#include <stdio.h>
/* additional interface header files */
#include "simplelink.h"
#include "BCDS_Basics.h"
#include "BCDS_Assert.h"
#include "OS_operatingSystem_ih.h"
#include "BCDS_WlanConnect.h"
#include "BCDS_NetworkConfig.h"
#include <Serval_Types.h>
#include <Serval_Basics.h>
#include <Serval_Ip.h>
#include "BCDS_NetworkConfig.h"

/* own header files */
#include "SendDataOverUdp.h"

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */

/**
 * This buffer holds the data to be sent to server via UDP
 * */
static uint16_t bsdBuffer_mau[BUFFER_SIZE] = { (uint16_t) ZERO };
/**
 * Timer handle for connecting to wifi and obtaining the IP address
 */
OS_timerHandle_tp wifiConnectTimerHandle_gdt = NULL;
/**
 * Timer handle for periodically sending data over wifi
 */
OS_timerHandle_tp wifiSendTimerHandle = NULL;

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */
/**
 *  @brief
 *      Function to initialize the wifi network send application. Create timer task
 *      to start WiFi Connect and get IP function after one second. After that another timer
 *      to send data periodically.
 */
void init(void)
{
	printf("begin (heart) \n");
    /* create timer task*/
    wifiConnectTimerHandle_gdt = OS_timerCreate((const int8_t *) "wifiConnect", 10*ONESECONDDELAY, OS_AUTORELOAD_OFF, NULL, wifiConnectGetIP);
    printf("wifi connected (heart) \n");
    wifiSendTimerHandle = OS_timerCreate((const int8_t *) "wifiSend", 10*ONESECONDDELAY, OS_AUTORELOAD_ON, NULL, wifiSend);
    printf("wifi connected success!!!\n");

    if ((wifiConnectTimerHandle_gdt != NULL) && (wifiSendTimerHandle != NULL))
    {
        /*start the wifi connect timer*/
        OS_timerStart(wifiConnectTimerHandle_gdt, TIMERBLOCKTIME);
    }
    else
    {
    /* Assertion Reason: "Failed to create timer task during initialization"   */
    assert(false);
    }

}

/**
 * @brief This is a template function where the user can write his custom application.
 *
 */
void appInitSystem(OS_timerHandle_tp xTimer)
{
    (void) (xTimer);
    /*Call the WNS module init API */
    init();
}


/**
 * @brief Opening a UDP client side socket and sending data on a server port
 *
 * This function opens a UDP socket and tries to connect to a Server SERVER_IP
 * waiting on port SERVER_PORT.
 * Then the function will send periodic UDP packets to the server.
 * 
 * @param[in] port
 *					destination port number
 *
 * @return         returnTypes_t:
 *                                  SOCKET_ERROR: when socket has not opened properly
 *                                  SEND_ERROR: when 0 transmitted bytes or send error
 *                                  STATUS_OK: when UDP sending was successful
 */


static returnTypes_t bsdUdpClient(uint16_t port)
{
	//printf("go into bsdUdpClient\n");
    static uint16_t counter = UINT16_C(0);
    SlSockAddrIn_t Addr;
    uint16_t AddrSize = (uint16_t) ZERO;
    int16_t SockID = (int16_t) ZERO;
    int16_t Status = (int16_t) ZERO;

/* copies the dummy data to send array , the first array element is the running counter to track the number of packets send so far*/
    bsdBuffer_mau[0] = (uint16_t)counter;
    bsdBuffer_mau[1] = 0xAA55;
    bsdBuffer_mau[2] = 0xBB66;
    bsdBuffer_mau[3] = 0xCC77;
    //char *cgl = "cgl";

    Addr.sin_family = SL_AF_INET;
    Addr.sin_port = sl_Htons((uint16_t) port);
    Addr.sin_addr.s_addr = sl_Htonl(SERVER_IP);
    //Addr.sin_addr.s_addr = sl_Htonl(SL_IPV4_VAL(204, 1, 168, 192));

    printf("server ip: %lu\n" , Addr.sin_addr.s_addr);
    printf("192.168.33.119 ip: %lu\n", sl_Htonl(SL_IPV4_VAL(119, 33, 168, 192)));
    //printf("server port: %hd\n" , Addr.sin_port);
    //Addr.sin_port = port;
    //Addr.sin_addr.s_addr = INADDR_ANY;
    AddrSize = sizeof(SlSockAddrIn_t);

    SockID = sl_Socket(SL_AF_INET, SL_SOCK_DGRAM, (uint32_t) ZERO); /**<The return value is a positive number if successful; other wise negative*/
    //printf("socket id: %d\n" , SockID);
    if (SockID < (int16_t) ZERO)
    {
        /* error case*/
        return (SOCKET_ERROR);
    }

    //Status = sl_SendTo(SockID, cgl, (strlen(cgl) + 1) * sizeof(char), (uint32_t) ZERO, (SlSockAddr_t *) &Addr, AddrSize);/**<The return value is a number of characters sent;negative if not successful*/

    Status = sl_SendTo(SockID, bsdBuffer_mau, (sizeof(bsdBuffer_mau)) * sizeof(uint16_t), (uint32_t) ZERO, (SlSockAddr_t *) &Addr, AddrSize);
    /*Check if 0 transmitted bytes sent or error condition*/
    //printf("status: %d\n" , Status);
    if (Status <= (int16_t) ZERO)
    {
        sl_Close(SockID);
        return (SEND_ERROR);
    }

    sl_Close(SockID);
    counter++;
    return (STATUS_OK);
}
/**
 *  @brief
 *      Function to periodically send data over WiFi as UDP packets. This is run as an Auto-reloading timer.
 *
 *  @param [in ] xTimer - necessary parameter for timer prototype
 */
static void wifiSend(OS_timerHandle_tp xTimer)
{
    (void) xTimer;
    if (STATUS_OK != bsdUdpClient(SERVER_PORT))
    {
      /* assertion Reason:  "Failed to  send udp packet" */
        assert(false);
    }
}

/**
 *  @brief
 *      Function to connect to wifi network and obtain IP address
 *
 *  @param [in ] xTimer
 */
static void wifiConnectGetIP(OS_timerHandle_tp xTimer)
{
    (void) xTimer;

    NCI_ipSettings_t myIpSettings;
    memset(&myIpSettings, (uint32_t) 0, sizeof(myIpSettings));
    int8_t ipAddress[PAL_IP_ADDRESS_SIZE] = {0};
    Ip_Address_T* IpaddressHex = Ip_getMyIpAddr();
    WLI_connectSSID_t connectSSID;
    WLI_connectPassPhrase_t connectPassPhrase;

    if(WLI_SUCCESS != WLI_init())
    {
        printf("Error occurred initializing WLAN \r\n ");
        return;
    }

    printf("Connecting to %s \r\n ",WLAN_CONNECT_WPA_SSID);

    connectSSID = (WLI_connectSSID_t) WLAN_CONNECT_WPA_SSID;
    connectPassPhrase = (WLI_connectPassPhrase_t) WLAN_CONNECT_WPA_PASS;

    if (WLI_SUCCESS == WLI_connectWPA(connectSSID, connectPassPhrase, NULL))
    {
        NCI_getIpSettings(&myIpSettings);
        *IpaddressHex = Basics_htonl(myIpSettings.ipV4);
        (void)Ip_convertAddrToString(IpaddressHex,(char *)&ipAddress);
        printf("Connected to WPA network successfully. \r\n ");
        printf("success!\n");
        printf(" Ip address of the device: %s \r\n ",ipAddress);
    }
    else
    {
        printf("Error occurred connecting %s \r\n ",WLAN_CONNECT_WPA_SSID);
        return;
    }


    /* After connection start the wifi sending timer*/
    OS_timerStart(wifiSendTimerHandle, TIMERBLOCKTIME);
    printf("start timerhandle\n");
}

/* global functions ********************************************************* */


/**
 * @brief API to de-initialize the WNS module
 */
void deinit(void)
{
    /*do nothing*/
}
/** ************************************************************************* */
