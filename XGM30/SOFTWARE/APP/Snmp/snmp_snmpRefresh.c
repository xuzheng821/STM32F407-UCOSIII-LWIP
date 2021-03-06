#include "Includes.h"
#include "arch/cc.h"
#include "lwip/api.h"
#include "Snmp\snmp_SNMP.h"
#include "Snmp\snmp_mib.h"
#include "lwip/inet.h"

#include "Config\config.h"
static u8_t bufftout[300];
static u8_t bufftin[150];

//u8_t    macAddress_id[] = {0x2b, 6, 1, 4, 1, 0x82, 0xb9, 0x6c, 1, 3, 2, 1, 1, 1, 0};
//u8_t    commonNELogicalID_id[] = {0x2b, 6, 1, 4, 1, 0x82, 0xb9, 0x6c, 1, 3, 1, 1, 0};
//u8_t    alarmLogInformation_id[] = {0x2b, 6, 1, 4, 1, 0x82, 0xb9, 0x6c, 1, 2, 3, 1, 2, 1};
//u8_t    alarmtext_id[] =  {0x2b, 6, 1, 4, 1, 0x82, 0xb9, 0x6c, 1, 2, 4, 0};
u8_t    sysUpTime_id[] = {0x2b, 6, 1, 2, 1, 1, 3, 0};
u8_t   traptype = 6;
u8_t   trapspecific = 1;
u8_t   traptrapv    = 1;         //snmp version
u32_t  TrapReq_ID   = 16777216;

u16_t analogValue[3];
s16_t discreteValue[8] = {1, 1, 1, 1, 1, 1, 1, 1};

extern u8_t commonNELogicalID[];
extern unsigned long SysTime;
extern u8_t commonNetworkAddress[];
extern unsigned char MACAddress[];
extern u8_t sysObjectID[];
extern u8_t snmp_Context[];

//extern const char *Str_fnDCPowerName[];

extern s16_t commonAlarmDetectionControl;
extern s16_t analogPropertyNO;                      //模拟属性表数目
extern s16_t currentAlarmNO;                        //当前报警表数目
extern s16_t discretePropertyNO;                    //离散属性表数目

extern AnalogPropertyTable analogPropertyTable[];

extern void snmp_TrapSend( u8_t* buf, u16_t len );
//-----------------------------------------------------------


extern s16_t    commonTamperStatus;
//*****************************************************************************
//
//! This structure instance contains the run-time set of configuration
//! parameters for S2E module.  This is the active parameter set and may
//! contain changes that are not to be committed to flash.
//
//*****************************************************************************
extern tConfigParameters g_sParameters;

//-------------------------------------------------------------
//发送模拟、离散属性陷阱例程，INDEX：在属性表中的位置，TYPE：告警类型
void SendTrap( int index, char type, char ana_dis )
{
    u8_t*   prevp;
    u8_t*   curp;
//    u8_t*   infoprevp;
//    u8_t*   infocurp;
//    s16_t   i, len;
//    s16_t   Seqlen = 0;
//    u8_t    DCTextptr;
    curp = prevp = bufftout + sizeof( bufftout );
    //alarmLogInformation
    memset( bufftin, 0, 4 );        //前四个时间字节设为0
    bufftin[4] = type;
    bufftin[5] = 0;
//    infocurp = infoprevp = bufftin + sizeof( bufftin );
    //  #if 1
    //  if(ana_dis==0)//模拟属性陷阱
    //  {
    ////        DCTextptr = fnNumberOpticalReceiver+fnNumberRFActives+2;   //4
    ////        if (index>=DCTextptr)                                      //4
    ////        {
    ////            snmpRWriteVal(&curp, (const u8_t *)Str_fnDCPowerName[index-DCTextptr], String, strlen((char*)(Str_fnDCPowerName[index-DCTextptr])));
    ////            snmpRWriteVal(&curp, (u8_t*)alarmtext_id, Identifier, 12);
    ////            snmpRWriteLength(&curp, Sequence, (u16_t)(prevp-curp-Seqlen));
    ////            Seqlen = (u16_t)(prevp-curp);
    ////        }
    //
    //      snmpRWriteInt(&infocurp, analogValue[index], Integer, 2);
    //      for (i=0; i<15; i++)
    //      {
    //          if(analogPropertyTable[index].analogParameterOID[i] == 0xff)break;
    //      }
    //          snmpRWriteVal(&infocurp, analogPropertyTable[index].analogParameterOID, Identifier, i);
    //  }
    //  else//离散属性陷阱
    //  {
    //      snmpRWriteInt(&infocurp, discreteValue[index], Integer, 2);
    //      for (i=0; i<15; i++)
    //      {
    //          if(discretePropertyTable[index].discreteParameterOID[i] == 0xff)break;
    //      }
    //      snmpRWriteVal(&infocurp, discretePropertyTable[index].discreteParameterOID, Identifier, i);
    //  }
    //    #endif
    //
    //
    //    len =(u16_t)(infoprevp-infocurp);
    //    for (i=0; i<len; i++)
    //          bufftin[i+6] = *(infocurp+i);
    //
    //    snmpRWriteVal(&curp, bufftin, OctetString, len+6);
    //    snmpRWriteVal(&curp, (u8_t*)alarmLogInformation_id, Identifier, 14);
    //    snmpRWriteLength(&curp, Sequence, (u16_t)(prevp-curp-Seqlen));
    //    Seqlen = (u16_t)(prevp-curp);
    //
    //    //commonNELogicalID
    //    snmpRWriteVal(&curp, commonNELogicalID, String, strlen((char*)commonNELogicalID));
    //    snmpRWriteVal(&curp, commonNELogicalID_id, Identifier, 13);
    //    snmpRWriteLength(&curp, Sequence, (u16_t)(prevp-curp-Seqlen));
    //    Seqlen = (u16_t)(prevp-curp);
    //
    //    //commonPhysicalAddress
    //    snmpRWriteVal(&curp, MACAddress, OctetString, 6);
    //    snmpRWriteVal(&curp, macAddress_id, Identifier, 15);
    //    snmpRWriteLength(&curp, Sequence, (u16_t)(prevp-curp-Seqlen));
    //
    //      /* Put sequence and lenth for variable-bindings */
    //    snmpRWriteLength(&curp, Sequence, prevp - curp);
//    /* Version 1 message places OID and upTime in PDU header */
//    snmpRWriteInt( &curp, SysTime, Ticks, 4 );
//    snmpRWriteInt( &curp, trapspecific, Integer, 1 );
//    snmpRWriteInt( &curp, traptype, Integer, 1 );
//    //snmpRWriteVal( &curp, commonNetworkAddress, IpAddress, 4 );
//    //snmpRWriteVal( &curp, sysObjectID, Identifier, 9 );
    
    
    if(ana_dis==0)//告警变化上报
    snmpRWriteVal(&curp, (const u8_t *)"SNMPV2 TRAP", String, 11);   
    snmpRWriteVal(&curp, (u8_t*)sysUpTime_id, Identifier, 9);
    snmpRWriteLength(&curp, Sequence, (u16_t)(prevp-curp));
    snmpRWriteLength(&curp, Sequence, prevp - curp);                              /* var bind         */
    snmpRWriteInt( &curp, 0, Integer, 1 );                                        /*error  code       */
    snmpRWriteInt( &curp, 0, Integer, 1 );                                        /*error index      */
    snmpRWriteInt( &curp, TrapReq_ID++, Integer, 4 );                               /*trap ID          */
    snmpRWriteLength( &curp, TrapV2, prevp - curp );                              /* pdu type        */
    snmpRWriteVal( &curp, snmp_Context, String, strlen( ( char* )snmp_Context ) );/*communicate serial*/
    snmpRWriteInt( &curp, traptrapv, Integer, 1 );                                /* msgVersion       */
    snmpRWriteLength( &curp, Sequence, prevp - curp );                            /* sequence*/
    /* Send trap to trap host */
    snmp_TrapSend( curp, prevp - curp );
    
    
}



void snmpTrapRefresh( void )
{
//    u8_t i, j;
//    static u8_t TrapEnable = 60;
//    s16_t temp;
//    s16_t currentAlarmNO_temp = 0;
//    if( TrapEnable < 60 ) //系统稳定大概30秒后开始发出trap告警
//    {
//        TrapEnable++;
//    }
//    else
//    {
//        // test snmpv2 trap.
//        SendTrap( i, 1, 0 );
//        TrapEnable = 0;
//    }
    {
      
//        //报警判断
//        if( commonAlarmDetectionControl == 1 )
//        {
//            return;    //不进行报警检测
//        }
//        currentAlarmNO_temp = 0;
//        for( i = 0; i < discretePropertyNO; i++ )
//        {
//            //告警允许
//            if( ( discretePropertyTable[i].discreteAlarmEnable == 2 ) || ( discretePropertyTable[i].discreteAlarmEnable == 3 ) )
//            {
//                if( discreteValue[i] == discretePropertyTable[i].discreteAlarmValue )
//                {
//                    //当前告警表处理
//                    for( j = 0; j < 15; j++ )
//                    {
//                        currentAlarmTable[currentAlarmNO_temp].currentAlarmOID[j] = discretePropertyTable[i].discreteParameterOID[j];
//                    }
//                    currentAlarmTable[currentAlarmNO_temp].currentAlarmState = discretePropertyTable[i].discreteAlarmEnable + 4;
//                    currentAlarmTable[currentAlarmNO_temp].currentAlarmValue = discretePropertyTable[i].discreteAlarmValue;
//                    currentAlarmNO_temp++;
//                    if( discretePropertyTable[i].discreteAlarmState ==
//                            discretePropertyTable[i].discreteAlarmEnable + 4 )
//                    {
//                        continue;
//                    }
//                    discretePropertyTable[i].discreteAlarmState =
//                        discretePropertyTable[i].discreteAlarmEnable + 4;
//                    //发送陷阱
//                    SendTrap( i, discretePropertyTable[i].discreteAlarmState, 1 );
//                    continue;
//                }
//            }
//            //告警解除
//            if( ( discretePropertyTable[i].discreteAlarmState == 6 ) ||
//                    ( discretePropertyTable[i].discreteAlarmState == 7 ) )
//            {
//                if( discreteValue[i] != discretePropertyTable[i].discreteAlarmValue )
//                {
//                    discretePropertyTable[i].discreteAlarmState = 1;
//                    //发送陷阱
//                    SendTrap( i, 1, 1 );
//                    continue;
//                }
//            }
//        }
//        for( i = 0; i < analogPropertyNO; i++ )
//        {
//            //HIHI告警允许
//            if( analogPropertyTable[i].alarmEnable[0] & 0x08 )   //HIHI
//            {
//                if( analogValue[i] >= analogPropertyTable[i].analogAlarmHIHI )
//                {
//                    //当前告警表处理
//                    for( j = 0; j < 15; j++ )
//                        currentAlarmTable[currentAlarmNO_temp].currentAlarmOID[j] =
//                            analogPropertyTable[i].analogParameterOID[j];
//                    currentAlarmTable[currentAlarmNO_temp].currentAlarmState = 2;
//                    currentAlarmTable[currentAlarmNO_temp].currentAlarmValue = analogValue[i];
//                    currentAlarmNO_temp++;
//                    if( analogPropertyTable[i].analogAlarmState == 2 )
//                    {
//                        continue;
//                    }
//                    analogPropertyTable[i].analogAlarmState = 2;
//                    //发送陷阱
//                    SendTrap( i, 2, 0 );
//                    continue;
//                }
//                //未发生HIHI告警，判断是否需要解除HIHI告警
//                if( analogPropertyTable[i].analogAlarmState == 2 )
//                {
//                    temp = analogPropertyTable[i].analogAlarmHIHI -
//                           analogPropertyTable[i].analogAlarmDeadband;
//                    if( analogValue[i] < temp )
//                    {
//                        //解除 HIHI告警
//                        analogPropertyTable[i].analogAlarmState = 1;
//                        //发送陷阱
//                        SendTrap( i, 1, 0 );
//                        continue;
//                    }
//                }
//            } //if (analogPropertyTable[i].alarmEnable[0] & 0x08)
//            //HI告警允许
//            if( analogPropertyTable[i].alarmEnable[0] & 0x04 )   //HI
//            {
//                if( analogValue[i] >= analogPropertyTable[i].analogAlarmHI )
//                {
//                    //当前告警表处理
//                    for( j = 0; j < 15; j++ )
//                        currentAlarmTable[currentAlarmNO_temp].currentAlarmOID[j] =
//                            analogPropertyTable[i].analogParameterOID[j];
//                    currentAlarmTable[currentAlarmNO_temp].currentAlarmState = 3;
//                    currentAlarmTable[currentAlarmNO_temp].currentAlarmValue = analogValue[i];
//                    currentAlarmNO_temp++;
//                    if( analogPropertyTable[i].analogAlarmState == 3 )
//                    {
//                        continue;
//                    }
//                    analogPropertyTable[i].analogAlarmState = 3;
//                    //发送陷阱
//                    SendTrap( i, 3, 0 );
//                    continue;
//                }
//                //未发生HI告警，判断是否需要解除HI告警
//                if( analogPropertyTable[i].analogAlarmState == 3 )
//                {
//                    temp = analogPropertyTable[i].analogAlarmHI -
//                           analogPropertyTable[i].analogAlarmDeadband;
//                    if( analogValue[i] < temp )
//                    {
//                        //解除 HI告警
//                        analogPropertyTable[i].analogAlarmState = 1;
//                        //发送陷阱
//                        SendTrap( i, 1, 0 );
//                        continue;
//                    }
//                }
//            } //if (analogPropertyTable[i].alarmEnable[0] & 0x04)
//            //LOLO告警允许
//            if( analogPropertyTable[i].alarmEnable[0] & 0x01 )   //LOLO
//            {
//                if( analogValue[i] <= analogPropertyTable[i].analogAlarmLOLO )
//                {
//                    //当前告警表处理
//                    for( j = 0; j < 15; j++ )
//                        currentAlarmTable[currentAlarmNO_temp].currentAlarmOID[j] =
//                            analogPropertyTable[i].analogParameterOID[j];
//                    currentAlarmTable[currentAlarmNO_temp].currentAlarmState = 5;
//                    currentAlarmTable[currentAlarmNO_temp].currentAlarmValue = analogValue[i];
//                    currentAlarmNO_temp++;
//                    if( analogPropertyTable[i].analogAlarmState == 5 )
//                    {
//                        continue;
//                    }
//                    analogPropertyTable[i].analogAlarmState = 5;
//                    //发送陷阱
//                    SendTrap( i, 5, 0 );
//                    continue;
//                }
//                //未发生LOLO告警，判断是否需要解除LOLO告警
//                if( analogPropertyTable[i].analogAlarmState == 5 )
//                {
//                    temp = analogPropertyTable[i].analogAlarmLOLO +
//                           analogPropertyTable[i].analogAlarmDeadband;
//                    if( analogValue[i] > temp )
//                    {
//                        //解除 LOLO告警
//                        analogPropertyTable[i].analogAlarmState = 1;
//                        //发送陷阱
//                        SendTrap( i, 1, 0 );
//                        continue;
//                    }
//                }
//            } //if (analogPropertyTable[i].alarmEnable[0] & 0x01)
//            //LO告警允许
//            if( analogPropertyTable[i].alarmEnable[0] & 0x02 )   //LO
//            {
//                if( analogValue[i] <= analogPropertyTable[i].analogAlarmLO )
//                {
//                    //当前告警表处理
//                    for( j = 0; j < 15; j++ )
//                        currentAlarmTable[currentAlarmNO_temp].currentAlarmOID[j] =
//                            analogPropertyTable[i].analogParameterOID[j];
//                    currentAlarmTable[currentAlarmNO_temp].currentAlarmState = 4;
//                    currentAlarmTable[currentAlarmNO_temp].currentAlarmValue = analogValue[i];
//                    currentAlarmNO_temp++;
//                    if( analogPropertyTable[i].analogAlarmState == 4 )
//                    {
//                        continue;
//                    }
//                    analogPropertyTable[i].analogAlarmState = 4;
//                    //发送陷阱
//                    SendTrap( i, 4, 0 );
//                    continue;
//                }
//                //未发生LO告警，判断是否需要解除LO告警
//                if( analogPropertyTable[i].analogAlarmState == 4 )
//                {
//                    temp = analogPropertyTable[i].analogAlarmLO +
//                           analogPropertyTable[i].analogAlarmDeadband;
//                    if( analogValue[i] > temp )
//                    {
//                        //解除 LO告警
//                        analogPropertyTable[i].analogAlarmState = 1;
//                        //发送陷阱
//                        SendTrap( i, 1, 0 );
//                        continue;
//                    }
//                }
//            } //if (analogPropertyTable[i].alarmEnable[0] & 0x02)
//        }
        
    }
//    currentAlarmNO = currentAlarmNO_temp;
    //
    // test snmpv2 trap.
    //SendTrap( i, 1, 0 );
}

//const s16_t dBm_Table[41] =
//{
//  -100,-90,-80,-70,-60,   -50,-40,-30,-20,-10,
//  0,
//  10,20,30,40,50,         60,70,80,90,100,
//  110,120,130,140,150,    160,170,180,190,200,
//  210,220,230,240,250,    260,270,280,290,300,
//};
//
//const u16_t mW_Table[41] =
//{
//  1,1,2,2,3,                  3,4,5,6,8,
//  10,
//  13,16,20,25,32,             40,50,63,79,100,
//  126,159,200,251,316,        398,501,631,794,1000,
//  1259,1585,1995,2512,3162,   3981,5012,6310,7943,10000,
//};
//u16_t dBmTomW(u16_t d)
//{
//  s16_t dBm;
//  u16_t mW = 0;
//  u8_t i,j;
//  u16_t offset = 0;
//
//  if(d&0x8000)
//  {
//      dBm = d & 0x7fff;
//      dBm = -dBm;
//  }
//  else
//  {
//      dBm = d;
//  }
//
//  if(dBm < 0)
//  {
//      if(dBm<=-100)
//      {
//          mW = 0;
//      }
//      else
//      {
//          j = 0;
//          for(i=0; i<=10; i++)
//          {
//              if(dBm <=  dBm_Table[i])
//              {
//                  j = i;
//                  break;
//              }
//          }
//          if(j!=0 && j!=10)
//          {
//              offset = (mW_Table[j]-mW_Table[j-1]) * (dBm_Table[j]-dBm)/(dBm_Table[j]-dBm_Table[j-1]);
//          }
//          mW = mW_Table[j] - offset;
//      }
//  }
//  else if(dBm > 0)
//  {
//      if(dBm>=300)
//      {
//          mW = 10000;
//      }
//      else
//      {
//          j = 10;
//          for(i=10; i<40; i++)
//          {
//              if(dBm <=  dBm_Table[i])
//              {
//                  j = i;
//                  break;
//              }
//          }
//          if(j!=10 && j !=39)
//          {
//              offset = (mW_Table[j]-mW_Table[j-1]) * (dBm_Table[j]-dBm)/(dBm_Table[j]-dBm_Table[j-1]);
//          }
//          mW = mW_Table[j] - offset;
//
//      }
//  }
//  else//dBm ==0
//  {
//      mW = 10;
//  }
//
//  return mW;
//
//}

//更新mib节点关联的数据,以及触发相应的trap
//
//! extern control main defintion .
//


//static const u8_t channel_status[4] = {0x01, 0x02, 0x04, 0x08};
//static const u8_t channel_fixtime_enable[4] = {0x10, 0x20, 0x40, 0x80};
//extern u16_t system_status;
extern u16_t commonNetworkPort;
void SnmpDataRefresh( void )
{
//    u8_t analogCount = 0;
//    u8_t discreteCount = 0;
//    u8_t i = 0;
//    u8_t en_fixtime = 0;
//    u32_t fixtick = 0;
//    u8_t bit_mark = 0;
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR cpu_sr;
#endif
//    //
//    //! common network address.
//    //
//    //AC overvoltage .
//    analogPropertyTable[0].analogAlarmHIHI = g_sParameters.OverVoltage;
//    analogPropertyTable[0].analogAlarmHI = g_sParameters.OverVoltage;
//    //leakage over current.
//    analogPropertyTable[1].analogAlarmHIHI = g_sParameters.LeakCurrent;
//    analogPropertyTable[1].analogAlarmHI = g_sParameters.LeakCurrent;
//    //AC load over current.
//    analogPropertyTable[2].analogAlarmHIHI = g_sParameters.OverCurrent;
//    analogPropertyTable[2].analogAlarmHI = g_sParameters.OverCurrent;
//    //AC UVLO
//    analogPropertyTable[0].analogAlarmLOLO = g_sParameters.UVLO ;
//    analogPropertyTable[0].analogAlarmLO = g_sParameters.UVLO ;
//    //regate count
//    fnRegateNumber = g_sParameters.rgcnt;
//    //regate delay1
//    fnRegateDelay1 = g_sParameters.delay1;
//    //regate delay2
//    fnRegateDelay2 = g_sParameters.delay2;
//    //regate delay3
//    fnRegateDelay3 = g_sParameters.delay3;
//    CPU_SR_ALLOC();
//    CPU_CRITICAL_ENTER();
//    //channel fixtime enable
//    for( i = 0; i < fnNumberOutput; i++ )
//    {
//        fixtick = g_sParameters.boottime[i].hour * 360000ul + g_sParameters.boottime[i].minute * 6000ul;
//        fnACoutputSWTable[i].fnOpenFixedTick = fixtick;
//        fixtick = g_sParameters.shuttime[i].hour * 360000ul + g_sParameters.shuttime[i].minute * 6000ul;
//        fnACoutputSWTable[i].fnCloseFixedTick = fixtick;
//        bit_mark = channel_status[i];
//        if( g_sParameters.remote_cmd & bit_mark )
//        {
//            fnACoutputSWTable[i].fnACOutputSWEnable = 1;
//        }
//        else
//        {
//            fnACoutputSWTable[i].fnACOutputSWEnable = 0;
//        }
//        en_fixtime = channel_fixtime_enable[i];
//        if( g_sParameters.remote_cmd & en_fixtime )
//        {
//            fnACoutputSWTable[i].fnFixedTimeSWEnable = 1;
//        }
//        else
//        {
//            fnACoutputSWTable[i].fnFixedTimeSWEnable = 0;
//        }
//    }
//    //sntp en
//    if( g_sParameters.ucFlags & SNTP_EN_FLAG )
//    {
//        fnSntpEn = 1;
//    }
//    else
//    {
//        fnSntpEn = 0;
//    }
//    //sntp inrer time
//    fnSntpInter = g_sParameters.SNTPInterval;
//    if( g_sParameters.ucFlags & CONFIG_EN_TRAP )
//    {
//        fnTrapEn = 1;
//    }
//    else
//    {
//        fnTrapEn = 0;
//    }
//    {
//        struct ip_addr ucTrapServiceIP;
//        char* pucurl = ( char* )g_sParameters.TrapService;
//        ucTrapServiceIP.addr = inet_addr( ( char* )pucurl );
//        fnTrapAddr[0] = ucTrapServiceIP.addr & 0xff;
//        fnTrapAddr[1] = ( ucTrapServiceIP.addr >> 8 ) & 0xff;
//        fnTrapAddr[2] = ( ucTrapServiceIP.addr >> 16 ) & 0xff;
//        fnTrapAddr[3] = ( ucTrapServiceIP.addr >> 24 ) & 0xff;
//        fnTrapPort = g_sParameters.TrapPort;
//    }
//    CPU_CRITICAL_EXIT();
        snmpTrapRefresh();
}

