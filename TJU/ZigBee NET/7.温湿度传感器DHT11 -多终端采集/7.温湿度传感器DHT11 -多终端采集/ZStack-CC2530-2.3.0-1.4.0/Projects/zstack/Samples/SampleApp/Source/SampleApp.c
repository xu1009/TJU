/**************************************************************************************************
  Filename:       SampleApp.c
  Revised:        $Date: 2009-03-18 15:56:27 -0700 (Wed, 18 Mar 2009) $
  Revision:       $Revision: 19453 $

  Description:    Sample Application (no Profile).


  Copyright 2007 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED 揂S IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/*********************************************************************
  This application isn't intended to do anything useful, it is
  intended to be a simple example of an application's structure.

  This application sends it's messages either as broadcast or
  broadcast filtered group messages.  The other (more normal)
  message addressing is unicast.  Most of the other sample
  applications are written to support the unicast message model.

  Key control:
    SW1:  Sends a flash command to all devices in Group 1.
    SW2:  Adds/Removes (toggles) this device in and out
          of Group 1.  This will enable and disable the
          reception of the flash command.
*********************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "ZGlobals.h"
#include "AF.h"
#include "aps_groups.h"
#include "ZDApp.h"

#include "SampleApp.h"
#include "SampleAppHw.h"

#include "OnBoard.h"

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "MT_UART.h"
#include "MT_APP.h"
#include "MT.h"
#include "DHT11.h"
#include "stdio.h"
#include "hal_adc.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
#define COM0_COM1_CALLBACK   0
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// This list should be filled with Application specific Cluster IDs.
const cId_t SampleApp_ClusterList[SAMPLEAPP_MAX_CLUSTERS] =
{
  SAMPLEAPP_PERIODIC_CLUSTERID,
  SAMPLEAPP_FLASH_CLUSTERID
};

const SimpleDescriptionFormat_t SampleApp_SimpleDesc =
{
  SAMPLEAPP_ENDPOINT,              //  int Endpoint;
  SAMPLEAPP_PROFID,                //  uint16 AppProfId[2];
  SAMPLEAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  SAMPLEAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  SAMPLEAPP_FLAGS,                 //  int   AppFlags:4;
  SAMPLEAPP_MAX_CLUSTERS,          //  uint8  AppNumInClusters;
  (cId_t *)SampleApp_ClusterList,  //  uint8 *pAppInClusterList;
  SAMPLEAPP_MAX_CLUSTERS,          //  uint8  AppNumInClusters;
  (cId_t *)SampleApp_ClusterList   //  uint8 *pAppInClusterList;
};

// This is the Endpoint/Interface description.  It is defined here, but
// filled-in in SampleApp_Init().  Another way to go would be to fill
// in the structure here and make it a "const" (in code space).  The
// way it's defined in this sample app it is define in RAM.
endPointDesc_t SampleApp_epDesc;

/*********************************************************************
 * EXTERNAL VARIABLES
 */
#if !defined( SERIAL_APP_TX_MAX )
#define SERIAL_APP_TX_MAX  80
#endif

#define SERIAL_APP_LOOPBACK  1
/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
uint8 SampleApp_TaskID;   // Task ID for internal task/event processing
                          // This variable will be received when
                          // SampleApp_Init() is called.
devStates_t SampleApp_NwkState;

uint8 SampleApp_TransID;  // This is the unique message ID (counter)

afAddrType_t SampleApp_Periodic_DstAddr; //广播
afAddrType_t SampleApp_Flash_DstAddr;    //组播
afAddrType_t SampleApp_P2P_DstAddr;      //点播

aps_Group_t SampleApp_Group;

uint8 SampleAppPeriodicCounter = 0;
uint8 SampleAppFlashCounter = 0;
static uint8 SerialApp_TxBuf[SERIAL_APP_TX_MAX+1];
static uint8 SerialApp_TxLen;
//此ID为不同终端的ID，
//分别为1，2，3，对应3个终端
uint8 deviceID=3;
float PM_25=123.3,PM_10=123.3;
uint16 CO2 = 1000.0;
//温度报警值
//超过这个值蜂鸣器报警
uint8 tempLimit=35;

//湿度报警
//小于这个值蜂鸣器报警
uint8 humLimit=30;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
void SampleApp_HandleKeys( uint8 shift, uint8 keys );
void SampleApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void SampleApp_SendPeriodicMessage( void );
void SampleApp_SendFlashMessage( uint16 flashTime );
void SampleApp_Send_P2P_Message(void);
void SetBuzzer(uint8 on);
/*********************************************************************
 * NETWORK LAYER CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SampleApp_Init
 *
 * @brief   Initialization function for the Generic App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
extern void macRadioUpdateTxPower(void);
void SampleApp_Init( uint8 task_id )
{  
  
  uint8 openPm[9] = {0xAA,0x01,0x00,0x00,0x00,0x00,0x01,0x66,0xbb};
 // uint8 setL[9] = {0xff,0x01,0x99,0x13,0x88,0x00,0x00,0x00,}
  SampleApp_TaskID = task_id;
  SampleApp_NwkState = DEV_INIT;
  SampleApp_TransID = 0;
  macRadioUpdateTxPower();
  MT_UartInit();                  //串口初始化
  MT_UartRegisterTaskID(task_id); //注册串口任务
 // HalUARTWrite(0,"Uart0Init OK\n", sizeof("Uart0Init OK\n"));//串口0发送
  //HalUARTWrite(1,"Uart1Init OK\n", sizeof("Uart1Init OK\n"));//串口1发送
  //协调器上接有一个蜂鸣器  
  P0SEL &= ~0x80;                  //设置P0.7口为普通IO
  P0DIR |= 0x80;                  //设置P0.7为输出
  SetBuzzer(0);
  
  HalUARTWrite(0,openPm,9);
    //MicroWait(2000);
  HalUARTWrite(0,openPm,9);
   // MicroWait(2000);
  
  // Device hardware initialization can be added here or in main() (Zmain.c).
  // If the hardware is application specific - add it here.
  // If the hardware is other parts of the device add it in main().

 #if defined ( BUILD_ALL_DEVICES )
  // The "Demo" target is setup to have BUILD_ALL_DEVICES and HOLD_AUTO_START
  // We are looking at a jumper (defined in SampleAppHw.c) to be jumpered
  // together - if they are - we will start up a coordinator. Otherwise,
  // the device will start as a router.
  if ( readCoordinatorJumper() )
    zgDeviceLogicalType = ZG_DEVICETYPE_COORDINATOR;
  else
    zgDeviceLogicalType = ZG_DEVICETYPE_ROUTER;
#endif // BUILD_ALL_DEVICES

#if defined ( HOLD_AUTO_START )
  // HOLD_AUTO_START is a compile option that will surpress ZDApp
  //  from starting the device and wait for the application to
  //  start the device.
  ZDOInitDevice(0);
#endif

  // Setup for the periodic message's destination address
  // Broadcast to everyone
  SampleApp_Periodic_DstAddr.addrMode = (afAddrMode_t)AddrBroadcast;
  SampleApp_Periodic_DstAddr.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_Periodic_DstAddr.addr.shortAddr = 0xFFFF;

  // Setup for the flash command's destination address - Group 1
  SampleApp_Flash_DstAddr.addrMode = (afAddrMode_t)afAddrGroup;
  SampleApp_Flash_DstAddr.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_Flash_DstAddr.addr.shortAddr = SAMPLEAPP_FLASH_GROUP;
  
  SampleApp_P2P_DstAddr.addrMode = (afAddrMode_t)Addr16Bit; //点播 
  SampleApp_P2P_DstAddr.endPoint = SAMPLEAPP_ENDPOINT; 
  SampleApp_P2P_DstAddr.addr.shortAddr = 0x0000;            //发给协调器

  // Fill out the endpoint description.
  SampleApp_epDesc.endPoint = SAMPLEAPP_ENDPOINT;
  SampleApp_epDesc.task_id = &SampleApp_TaskID;
  SampleApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&SampleApp_SimpleDesc;
  SampleApp_epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
  afRegister( &SampleApp_epDesc );

  // Register for all key events - This app will handle all key events
  RegisterForKeys( SampleApp_TaskID );

  // By default, all devices start out in Group 1
  SampleApp_Group.ID = 0x0001;
  osal_memcpy( SampleApp_Group.name, "Group 1", 7 );
  aps_AddGroup( SAMPLEAPP_ENDPOINT, &SampleApp_Group );

#if defined ( LCD_SUPPORTED )
  HalLcdWriteString( "SampleApp", HAL_LCD_LINE_1 );
#endif
}

/*********************************************************************
 * @fn      SampleApp_ProcessEvent
 *
 * @brief   Generic Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */
uint16 SampleApp_ProcessEvent( uint8 task_id, uint16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  (void)task_id;  // Intentionally unreferenced parameter

  if ( events & SYS_EVENT_MSG )
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {
        // Received when a key is pressed
        case KEY_CHANGE:
          SampleApp_HandleKeys( ((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
          break;

        // Received when a messages is received (OTA) for this endpoint
        case AF_INCOMING_MSG_CMD:
          SampleApp_MessageMSGCB( MSGpkt );
          break;

        // Received whenever the device changes state in the network
        case ZDO_STATE_CHANGE:
          SampleApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if ( //(SampleApp_NwkState == DEV_ZB_COORD) ||
                 (SampleApp_NwkState == DEV_ROUTER)
              || (SampleApp_NwkState == DEV_END_DEVICE) )
          {
            // Start sending the periodic message in a regular interval.
            osal_start_timerEx( SampleApp_TaskID,
                              SAMPLEAPP_SEND_PERIODIC_MSG_EVT,
                              SAMPLEAPP_SEND_PERIODIC_MSG_TIMEOUT );
          }
          else
          {
            // Device is no longer in the network
          }
          break;

        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next - if one is available
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( SampleApp_TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }

  // Send a message out - This event is generated by a timer
  //  (setup in SampleApp_Init()).
  if ( events & SAMPLEAPP_SEND_PERIODIC_MSG_EVT )   // 定时器溢出之后产生该事件 
  {
    // Send the periodic message
    //SampleApp_SendPeriodicMessage();
    SampleApp_Send_P2P_Message();

    // Setup to send message again in normal period (+ a little jitter)
    osal_start_timerEx( SampleApp_TaskID, SAMPLEAPP_SEND_PERIODIC_MSG_EVT,
        (SAMPLEAPP_SEND_PERIODIC_MSG_TIMEOUT + (osal_rand() & 0x00FF)) );

    // return unprocessed events
    return (events ^ SAMPLEAPP_SEND_PERIODIC_MSG_EVT);
  }

  // Discard unknown events
  return 0;
}

/*********************************************************************
 * Event Generation Functions
 */
/*********************************************************************
 * @fn      SampleApp_HandleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 HAL_KEY_SW_2
 *                 HAL_KEY_SW_1
 *
 * @return  none
 */
void SampleApp_HandleKeys( uint8 shift, uint8 keys )
{
  (void)shift;  // Intentionally unreferenced parameter
  
  if ( keys & HAL_KEY_SW_1 )
  {
    /* This key sends the Flash Command is sent to Group 1.
     * This device will not receive the Flash Command from this
     * device (even if it belongs to group 1).
     */
    SampleApp_SendFlashMessage( SAMPLEAPP_FLASH_DURATION );
  }

  if ( keys & HAL_KEY_SW_2 )
  {
    /* The Flashr Command is sent to Group 1.
     * This key toggles this device in and out of group 1.
     * If this device doesn't belong to group 1, this application
     * will not receive the Flash command sent to group 1.
     */
    aps_Group_t *grp;
    grp = aps_FindGroup( SAMPLEAPP_ENDPOINT, SAMPLEAPP_FLASH_GROUP );
    if ( grp )
    {
      // Remove from the group
      aps_RemoveGroup( SAMPLEAPP_ENDPOINT, SAMPLEAPP_FLASH_GROUP );
    }
    else
    {
      // Add to the flash group
      aps_AddGroup( SAMPLEAPP_ENDPOINT, &SampleApp_Group );
    }
  }
}

/*********************************************************************
 * LOCAL FUNCTIONS
 */

void SetBuzzer(uint8 on)
{
  if(on>0)
  {
    P0_7=0;
  }
  else
  {
    P0_7=1;    
  }
}

/*********************************************************************
 * @fn      SampleApp_MessageMSGCB
 *
 * @brief   Data message processor callback.  This function processes
 *          any incoming data - probably from other devices.  So, based
 *          on cluster ID, perform the intended action.
 *
 * @param   none
 *
 * @return  none
 */
void SampleApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )   //协调器接收
{
  uint16 flashTime;
  uint8 dataOfGet[50]={0};
  uint8 buff[40]={0};
  uint8 index=0,temp=0, humidity=0;
  uint8 bAlarm=0;
  switch ( pkt->clusterId )  
  {
    case SAMPLEAPP_P2P_CLUSTERID:
      {  
      //  HalUARTWrite(0, pkt->cmd.Data, strlen(pkt->cmd.Data)); 
       // HalUARTWrite(0, dataOfGet, 130); 
         dataOfGet[0]='I';
         dataOfGet[1]='D';
         dataOfGet[33]=0x0d;
         dataOfGet[34]=0x0a;
    // dataOfGet[2]=pkt->cmd.Data[0];//取出终端的索引号
     
   //  HalUARTWrite(0, pkt->cmd.Data, 1);
  //   if(index=='3')
    // {
       //  HalUARTWrite(0,"get", 3);
           for(int i = 0;i<31;i++)
         { 
           dataOfGet[i+2]=pkt->cmd.Data[i];
         }
        HalUARTWrite(0, dataOfGet, 35); 
     
   //  }
//      switch(index)
//      {
//      case 0x31:
//        for(int i = 2;i<27;i++)
//        {dataOfGet[i]=pkt->cmd.Data[i-1];}
//        break;
//      case 0x32:
//         for(int i = 27;i<52;i++)
//         {dataOfGet[i]=pkt->cmd.Data[i-27];}
//        break;
//      case 0x33:
//         for(int i = 52;i<77;i++)
//         { dataOfGet[i]=pkt->cmd.Data[i-52];}
//        break;
//      case 0x34:
//        for(int i = 77;i<102;i++)
//        {  dataOfGet[i]=pkt->cmd.Data[i-77];}
//        break;
//      case 0x35:
//         for(int i = 102;i<127;i++)
//         {  dataOfGet[i]=pkt->cmd.Data[i-102];}
//        break;
//      //default:
//       // break;   
//      } 
      //  HalUARTWrite(0, dataOfGet, strlen(dataOfGet)); 
      break;    
      }
    case SAMPLEAPP_PERIODIC_CLUSTERID:
      break;

    case SAMPLEAPP_FLASH_CLUSTERID:
      flashTime = BUILD_UINT16(pkt->cmd.Data[1], pkt->cmd.Data[2] );
      HalLedBlink( HAL_LED_4, 4, 50, (flashTime / 4) );
      break;
  }
}
void SerialApp_CallBack(uint8 port, uint8 event)
{
  (void)port;

    if ((event & (HAL_UART_RX_FULL | HAL_UART_RX_ABOUT_FULL | HAL_UART_RX_TIMEOUT)))
    {
        if (SerialApp_TxLen < SERIAL_APP_TX_MAX)
        {
            SerialApp_TxLen = HalUARTRead(port, SerialApp_TxBuf, SERIAL_APP_TX_MAX);
            if (SerialApp_TxLen)
            {
#if COM0_COM1_CALLBACK            
                //收到哪个串口的数据就从写回到哪个串口中去
                //我们做的是回
                HalUARTWrite(port, SerialApp_TxBuf, SerialApp_TxLen);
#else
            //双串口互错传输
            //串口0发---> 串口1收
            //串口1发---> 串口0收

                if(port==1)     //串口1
                {
                 if(SerialApp_TxBuf[0]==0xff&&SerialApp_TxBuf[1]==0x86)
                 {
                 
                 
                if(((0xff-((SerialApp_TxBuf[1]+SerialApp_TxBuf[2]+SerialApp_TxBuf[3]+SerialApp_TxBuf[4]+SerialApp_TxBuf[5]+SerialApp_TxBuf[6]+SerialApp_TxBuf[7])&0xff))+1)==SerialApp_TxBuf[8])
                 {
                 CO2 = SerialApp_TxBuf[2]*256+SerialApp_TxBuf[3];
                 
                 }
                 
                 }
                   //HalUARTWrite(1, SerialApp_TxBuf, SerialApp_TxLen);
                }
                else    //串口0
                  
                {
                 //  HalUARTWrite(0, SerialApp_TxBuf, SerialApp_TxLen);
                   //HalUARTWrite(0, "get", 3);
                   if(SerialApp_TxBuf[0]==0xaa&&SerialApp_TxBuf[1]==0x02)
                   {
                      //HalUARTWrite(0, SerialApp_TxBuf, SerialApp_TxLen);
                     //  HalUARTWrite(0, "getit", 5);
                     if((SerialApp_TxBuf[0]+SerialApp_TxBuf[1]+SerialApp_TxBuf[2]+SerialApp_TxBuf[3]+SerialApp_TxBuf[4]+SerialApp_TxBuf[5]+SerialApp_TxBuf[8])==(SerialApp_TxBuf[6]*256+SerialApp_TxBuf[7]))
                     {
                       
                     PM_25 = (SerialApp_TxBuf[4])*256+(SerialApp_TxBuf[5]);
                     PM_10 = (SerialApp_TxBuf[2])*256+(SerialApp_TxBuf[3]);
                   
                     }
                   
                   }
                }
#endif
             }            

            SerialApp_TxLen = 0;
        }
    }
}
/*********************************************************************
 * @fn      SampleApp_SendPeriodicMessage
 *
 * @brief   Send the periodic message.
 *
 * @param   none
 *
 * @return  none
 */
void SampleApp_SendPeriodicMessage( void )
{
  if ( AF_DataRequest( &SampleApp_Periodic_DstAddr, &SampleApp_epDesc,
                       SAMPLEAPP_PERIODIC_CLUSTERID,
                       1,
                       (uint8*)&SampleAppPeriodicCounter,
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}

/*********************************************************************
 * @fn      SampleApp_SendFlashMessage
 *
 * @brief   Send the flash message to group 1.
 *
 * @param   flashTime - in milliseconds
 *
 * @return  none
 */
void SampleApp_SendFlashMessage( uint16 flashTime )
{
  uint8 buffer[3];
  buffer[0] = (uint8)(SampleAppFlashCounter++);
  buffer[1] = LO_UINT16( flashTime );
  buffer[2] = HI_UINT16( flashTime );

  if ( AF_DataRequest( &SampleApp_Flash_DstAddr, &SampleApp_epDesc,
                       SAMPLEAPP_FLASH_CLUSTERID,
                       3,
                       buffer,
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}

/*********************************************************************
 * @fn      SampleApp_Send_P2P_Message
 *
 * @brief   point to point.
 *
 * @param   none
 *
 * @return  none
 */
void SampleApp_Send_P2P_Message( void )   // 子节点发送信息到协调器数据在此汇总，发送周期由定时器决定
{
 //MicroWait(200); 
 
  uint8 readData[9] = {0xaa,0x02,0x00,0x00,0x00,0x00,0x01,0x67,0xbb};
  uint8 readCo2[9] =  {0xff,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
  uint8 strTemp[50]={0};
  //uint8 wnd[4] = {0};
  float vol=0.0,fengya=-12.3; //adc采样电压  
  float temp=0.0; //保存温度
  float humidity=0.0;//保存湿度
  //uint8 start1 = 'I';
 // uint8 start2 = 'D';
  
 

 HalUARTWrite(0,readData,9);
 // MicroWait(200);
 HalUARTWrite(0,readData,9);
 MicroWait(200);
 
 HalUARTWrite(1,readCo2,9);
 // MicroWait(200);
 HalUARTWrite(1,readCo2,9);
  
 MicroWait(200);
  //MicroWait(200);
  uint16 adc= HalAdcRead(HAL_ADC_CHANNEL_6, HAL_ADC_RESOLUTION_14);  // P0_6
   //最大采样值8192(因为最高位是符号位)
    if(adc>=8192)
    {
       // sprintf(&str_adc[6], "error");
      //  len=11;
    }
    else
    {
       //输出电压
        vol=(float)((float)(adc*3.3))/8192.0;
        if(vol<0.4||vol>2.0)
        {
        }
        else
        {
        
        vol = 125*(vol-0.4)-100;
        
        }
        //取小数点的3位，放大1000倍
        adc=vol*1000;
        
    
    }
  DHT11();             //获取温湿度
  temp=wendu_shi*10+wendu_ge+((float)(wendu_p))/10;//根据十位和个位计算温度
  humidity=shidu_shi*10+shidu_ge+((float)(shidu_p))/10;//根据十位和个位计算湿度  
  //显示当前终端
 // sprintf(strTemp, "终端%d", deviceID);
  //HalLcdWriteString( strTemp, HAL_LCD_LINE_3 );

  //格式化温湿度同，并LCD显示
  //sprintf(strTemp, "温度:%02d 湿度:%02d", temp, humidity);
  //HalLcdWriteString( strTemp, HAL_LCD_LINE_4 );

  //串口输出数据
  //HalUARTWrite(0,strTemp,osal_strlen(strTemp));
 // HalUARTWrite(0, "\n",1);

  //格式化数据，用于无线发送
//  strTemp[0]=deviceID;//增加终端的ID到无线信息上
//  
//  strTemp[1] = 12;   //风压
//  strTemp[2] = 3;   //风压
//  
//  strTemp[2] = 12;  //pm2.5
//  strTemp[3] = 34;  //pm2.5
//  
//  strTemp[4] = 12;  //pm10
//  strTemp[5] = 34;  //pm10
//  
//  strTemp[6]=humidity;//湿度
//  strTemp[7] = shidu_p;  //湿度
//  
//  strTemp[8]=temp;//温度
//  strTemp[9]=wendu_p;//温度
  sprintf(strTemp, "%d%05.1f%05.1f%05.1f%05.1f%05.1f%05d", deviceID, fengya,PM_25,PM_10,humidity,temp,CO2);
  if(temp>0)
  if ( AF_DataRequest( &SampleApp_P2P_DstAddr, &SampleApp_epDesc,
                       SAMPLEAPP_P2P_CLUSTERID,
                       31, //发送长度
                       strTemp,//发送指针
                       &SampleApp_TransID,
                       AF_DISCV_ROUTE,
                       AF_DEFAULT_RADIUS ) == afStatus_SUCCESS )
  {
  }
  else
  {
    // Error occurred in request to send.
  }
}
/*********************************************************************
*********************************************************************/
