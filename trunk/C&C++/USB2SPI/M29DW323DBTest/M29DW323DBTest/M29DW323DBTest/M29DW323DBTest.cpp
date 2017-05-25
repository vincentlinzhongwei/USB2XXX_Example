  /*
  ******************************************************************************
  * @file     : USB2XXXSPIADS1256.cpp
  * @Copyright: usbxyz 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 9:33
  * @brief    : USB2XXX SPI M95128 test demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, usbxyz.com
  * http://www.usbxyz.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "usb_device.h"
#include "usb2m29dw.h"
#include "usb2gpio.h"
#include "usb2spi.h"
/*
Ӳ������˵��
USB2XXX     ADS1256
--------------------
P0          DRDY
P1          RESET
P4          CS
P5          SCLK
P6          DOUT
P7          DIN
*/


#define SPI1_CS0    (0x00)
#define SPI1_CS1    (0x10)
#define WRITE_SPI_INDEX     SPI1_CS1
#define READ_SPI_INDEX      SPI1_CS0
#define WEN_GPIO            (1<<15) //Write Enable���͵�ƽ��Ч
#define OEN_GPIO            (1<<8)  //Output Enable,�͵�ƽ��Ч
#define RP_GPIO             (1<<4)  //Reset/Block Temporary Unprotect
#define RB_GPIO             (1<<5)  //Ready/Busy Output
#define CE_GPIO             (1<<9)  //Chip Enable,�͵�ƽ��Ч

int DevHandle[10];

bool Init(void)
{
    DEVICE_INFO DevInfo;
    bool state;
    int ret;
    //ɨ������豸
    ret = USB_ScanDevice(DevHandle);
    if(ret <= 0){
        printf("No device connected!\n");
        return false;
    }
    //���豸
    state = USB_OpenDevice(DevHandle[0]);
    if(!state){
        printf("Open device error!\n");
        return false;
    }
    //��ȡ�̼���Ϣ
    char FuncStr[128]={0};
    state = DEV_GetDeviceInfo(DevHandle[0],&DevInfo,FuncStr);
    if(!state){
        printf("Get device infomation error!\n");
        return false;
    }else{
        printf("Firmware Name:%s\n",DevInfo.FirmwareName);
        printf("Firmware Build Date:%s\n",DevInfo.BuildDate);
        printf("Firmware Version:v%d.%d.%d\n",(DevInfo.FirmwareVersion>>24)&0xFF,(DevInfo.FirmwareVersion>>16)&0xFF,DevInfo.FirmwareVersion&0xFFFF);
        printf("Hardware Version:v%d.%d.%d\n",(DevInfo.HardwareVersion>>24)&0xFF,(DevInfo.HardwareVersion>>16)&0xFF,DevInfo.HardwareVersion&0xFFFF);
        printf("Firmware Functions:%s\n",FuncStr);
    }
    //��ʼ��SPI
    //����SPI������ز���
    SPI_CONFIG SPIConfig;
    SPIConfig.Mode = SPI_MODE_HARD_HDX;//��оƬ֧�ֵ�ʱ��Ƶ�ʽϵͣ�����ʹ������ģʽ
    SPIConfig.ClockSpeedHz = 25000000;
    SPIConfig.CPHA = 0;
    SPIConfig.CPOL = 0;
    SPIConfig.LSBFirst = SPI_MSB;
    SPIConfig.Master = SPI_MASTER;
    SPIConfig.SelPolarity = SPI_SEL_LOW;
    ret = SPI_Init(DevHandle[0],SPI1,&SPIConfig);
    if(ret != SPI_SUCCESS){
        printf("Initialize device error!\n");
        return false;
    }
    //��ʼ�����ÿ�������
    ret = GPIO_SetOutput(DevHandle[0],WEN_GPIO|OEN_GPIO|RP_GPIO|CE_GPIO,GPIO_PUPD_UP);
    if(ret != GPIO_SUCCESS){
        printf("Initialize gpio error!\n");
        return false;
    }
    ret = GPIO_SetInput(DevHandle[0],RB_GPIO,GPIO_PUPD_UP);
    if(ret != GPIO_SUCCESS){
        printf("Initialize gpio error!\n");
        return false;
    }
    return true;
}

bool Write(uint32_t Addr,uint16_t Data)
{
    uint8_t write_buffer[]={Addr>>16,Addr>>8,Addr,Data>>8,Data};
    GPIO_Write(DevHandle[0],OEN_GPIO,OEN_GPIO);
    GPIO_Write(DevHandle[0],CE_GPIO,0x0000);
    int ret = SPI_WriteBytes(DevHandle[0],WRITE_SPI_INDEX,write_buffer,sizeof(write_buffer));
    if(ret != SPI_SUCCESS){
        printf("Write data error!\n");
        return false;
    }
    GPIO_Write(DevHandle[0],WEN_GPIO,0x0000);
    GPIO_Write(DevHandle[0],WEN_GPIO,WEN_GPIO);
    GPIO_Write(DevHandle[0],CE_GPIO,CE_GPIO);
    GPIO_Write(DevHandle[0],OEN_GPIO,0x0000);
}

bool Read(uint32_t Addr,uint16_t *pData)
{
    uint8_t write_buffer[]={Addr>>16,Addr>>8,Addr,0,0};
    uint8_t read_buffer[2];
    GPIO_Write(DevHandle[0],CE_GPIO,0x0000);
    int ret = SPI_WriteBytes(DevHandle[0],WRITE_SPI_INDEX,write_buffer,sizeof(write_buffer));
    if(ret != SPI_SUCCESS){
        printf("Write data error!\n");
        return false;
    }
    GPIO_Write(DevHandle[0],OEN_GPIO,0x0000);
    GPIO_Write(DevHandle[0],WEN_GPIO,WEN_GPIO);
    GPIO_Write(DevHandle[0],OEN_GPIO,OEN_GPIO);
    ret = SPI_ReadBytes(DevHandle[0],READ_SPI_INDEX,read_buffer,2);
    if(ret != SPI_SUCCESS){
        printf("Read data error!\n");
        return false;
    }
    *pData = (read_buffer[0]<<8)|read_buffer[1];
    return true;
}

//������
int main(int argc, const char* argv[])
{
    
    DEVICE_INFO DevInfo;
    bool state;
    int ret;
    //ɨ������豸
    ret = USB_ScanDevice(DevHandle);
    if(ret <= 0){
        printf("No device connected!\n");
        return 0;
    }
    //���豸
    state = USB_OpenDevice(DevHandle[0]);
    if(!state){
        printf("Open device error!\n");
        return 0;
    }
    //��ȡ�̼���Ϣ
    char FuncStr[128]={0};
    state = DEV_GetDeviceInfo(DevHandle[0],&DevInfo,FuncStr);
    if(!state){
        printf("Get device infomation error!\n");
        return 0;
    }else{
        printf("Firmware Name:%s\n",DevInfo.FirmwareName);
        printf("Firmware Build Date:%s\n",DevInfo.BuildDate);
        printf("Firmware Version:v%d.%d.%d\n",(DevInfo.FirmwareVersion>>24)&0xFF,(DevInfo.FirmwareVersion>>16)&0xFF,DevInfo.FirmwareVersion&0xFFFF);
        printf("Hardware Version:v%d.%d.%d\n",(DevInfo.HardwareVersion>>24)&0xFF,(DevInfo.HardwareVersion>>16)&0xFF,DevInfo.HardwareVersion&0xFFFF);
        printf("Firmware Functions:%s\n",FuncStr);
    }
    /*Init();
    uint16_t Data;
    for(int i=0;i<10;i++){
        Write(i,i);
        Sleep(10);
        Read(i,&Data);
        printf("%04X ",Data);
        Sleep(10);
    }
    return 0;*/
    uint16_t WriteDataBuffer[20480];
    uint16_t ReadDataBuffer[20480];
    int TestDataNum = 10240;
    for(int i=0;i<TestDataNum;i++){
        WriteDataBuffer[i] = rand();
    }

    ret = M29DW_Init(DevHandle[0]);
    if(ret != M29DW_SUCCESS){
        printf("Init error!\n");
        return 0;
    }

    printf("Erase block...\n");
    for(int i=0;i<=(TestDataNum/0x1000);i++){
        ret = M29DW_BlockErase(DevHandle[0],i*0x1000);
        if(ret != M29DW_SUCCESS){
            printf("Block Erase error!\n");
            return 0;
        }
    }
    
    printf("Write data...\n");
    ret = M29DW_WriteData(DevHandle[0],0,WriteDataBuffer,TestDataNum);
    if(ret != M29DW_SUCCESS){
        printf("Write data error!\n");
        return 0;
    }
    printf("Read data...\n");
    ret = M29DW_ReadData(DevHandle[0],0,ReadDataBuffer,TestDataNum);
    if(ret != M29DW_SUCCESS){
        printf("Read data error!\n");
        return 0;
    }

    int ErrorData = 0;
    for(int i=0;i<TestDataNum;i++){
        if(WriteDataBuffer[i] != ReadDataBuffer[i]){
            ErrorData++;
        }
    }
    printf("ErrorData = %d\n",ErrorData);
    printf("\nM29DW323DB Test OK\n");
    getchar();
    //�ر��豸
    USB_CloseDevice(DevHandle[0]);
    return 0;
}