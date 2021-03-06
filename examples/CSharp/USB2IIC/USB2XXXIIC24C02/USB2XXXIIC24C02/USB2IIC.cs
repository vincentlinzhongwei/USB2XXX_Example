﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace USB2XXX
{
    class USB2IIC
    {
        //定义函数返回错误代码
        public const Int32 IIC_SUCCESS             = (0);   //函数执行成功
        public const Int32 IIC_ERR_NOT_SUPPORT     = (-1);  //适配器不支持该函数
        public const Int32 IIC_ERR_USB_WRITE_FAIL  = (-2);  //USB写数据失败
        public const Int32 IIC_ERR_USB_READ_FAIL   = (-3);  //USB读数据失败
        public const Int32 IIC_ERR_CMD_FAIL        = (-4);  //命令执行失败
        public const Int32 IIC_ERR_PARA_ERROR      = (-5);  //参数传入错误
        //定义IIC函数返回错误代码
        public const Int32 IIC_ERROR_SUCCESS     = 0;   //操作成功
        public const Int32 IIC_ERROR_CHANNEL     = 1;   //该通道不支持该函数
        public const Int32 IIC_ERROR_BUSY        = 2;   //总线忙
        public const Int32 IIC_ERROR_START_FAILD = 3;   //启动总线失败
        public const Int32 IIC_ERROR_TIMEOUT     = 4;   //超时
        public const Int32 IIC_ERROR_NACK        = 5;   //从机无应答
        //定义初始化IIC的数据类型
        public struct IIC_CONFIG
        {
          public UInt32 ClockSpeedHz; //IIC时钟频率:单位为Hz
          public UInt16 OwnAddr;      //USB2XXX为从机时自己的地址
          public Byte   Master;       //主从选择控制:0-从机，1-主机
          public Byte   AddrBits;     //从机地址模式，7-7bit模式，10-10bit模式
        }
        //USB2IIC相关函数定义
        [DllImport("USB2XXX.dll")]
        public static extern Int32 IIC_Init(Int32 DevIndex,Int32 IICIndex, ref IIC_CONFIG pConfig);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 IIC_GetSlaveAddr(Int32 DevIndex,Int32 IICIndex,Int16[] pSlaveAddr);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 IIC_WriteBytes(Int32 DevIndex,Int32 IICIndex,Int16 SlaveAddr,Byte[] pWriteData,Int32 WriteLen,Int32 TimeOutMs);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 IIC_ReadBytes(Int32 DevIndex,Int32 IICIndex,Int16 SlaveAddr,Byte[] pReadData,Int32 ReadLen,Int32 TimeOutMs);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 IIC_WriteReadBytes(Int32 DevIndex,Int32 IICIndex,Int16 SlaveAddr,Byte[] pWriteData,Int32 WriteLen,Byte[] pReadData,Int32 ReadLen,Int32 TimeOutMs);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 IIC_SlaveWriteBytes(Int32 DevIndex,Int32 IICIndex,Byte[] pWriteData,Int32 WriteLen,Int32 TimeOutMs);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 IIC_SlaveReadBytes(Int32 DevIndex,Int32 IICIndex,Byte[] pReadData,Int32 TimeOutMs);
        [DllImport("USB2XXX.dll")]
        public static extern Int32 IIC_SlaveWriteRemain(Int32 DevIndex,Int32 IICIndex);
    }
}
