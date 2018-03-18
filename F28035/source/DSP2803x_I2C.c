// TI File $Revision: /main/2 $
// Checkin $Date: December 2, 2004   11:55:13 $
//###########################################################################
//
// FILE:	DSP280x_I2C.c
//
// TITLE:	DSP280x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP280x V1.30 $
// $Release Date: February 10, 2006 $
//###########################################################################

#include "DSP2803x_Device.h"     // DSP280x Headerfile Include File

//---------------------------------------------------------------------------
// Example: InitI2CGpio: 
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as I2C pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.  
// 
// Caution: 
// Only one GPIO pin should be enabled for SDAA operation.
// Only one GPIO pin shoudl be enabled for SCLA operation. 
// Comment out other unwanted lines.

void InitI2CGpio()
{

   EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.  
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)

/* Configure SCI pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be I2C functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation
	
    EDIS;
	//GpioDataRegs.GPASET.bit.GPIO23 = 1;	//WP=1,disable write
}
//---------------------------------------------------------------------------
// InitI2C: 
//---------------------------------------------------------------------------
// This function initializes the I2C to a known state.
//
void InitI2C(void)
{
	/*InitI2CGpio();

	// Initialize I2C
	I2caRegs.I2CMDR.bit.IRS = 0;
	I2caRegs.I2CSAR = 0x0050;	// 7Fh,the 7-bit slave addr that  I2C transmits
								// when it is in the master-transmitter mode
	I2caRegs.I2CPSC.all = 5;	// 10M,Prescaler - need 7-12 Mhz on module clk
	I2caRegs.I2CCLKL = 40;		// ICCL,NOTE: must be non zero
	I2caRegs.I2CCLKH = 35;		// ICCH,NOTE: must be non zero
	I2caRegs.I2CIER.all = 0x00;	// Disable SCD & ARDY interrupts
	 */ 
	 /* 
		                           60MHz      60M
	  module clock frequency = ----------- = ------=10MHz
		                         (IPSC+1)       6

	                                   60MHz               		 60MHz
	  master clock frequency = ---------------------------	 = -------- = 80K
		                         (IPSC+1)[(ICCL+5)+(ICCH+5)]     6*125
	*/
	/*
	I2caRegs.I2CMDR.bit.IRS = 1;	// Take I2C out of reset
   									// Stop I2C when suspended
	I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
	I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,

	I2caRegs.I2CCNT = 0x0000;
	I2caRegs.I2CSTR.all = 0xFFFF;
*/
    INT16 i16Temp; 
	InitI2CGpio();
	
	I2caRegs.I2CMDR.bit.IRS = 0;								//I2Cģ�鲻ʹ��
//	I2caRegs.I2CSAR = 0x50;									// ����EEPROM Ƭ��ַ
	
	// I2Cʱ�ӷ�Ƶ����ǰʱ��Ϊ60000000/7/125=68.6kHz
	//when I2CPSC >=1 ʱ��=ϵͳʱ��/(I2CPSC+1)/((I2CCLKL+5) +(I2CCLKH+5)) 
	//when I2CPSC =1   ʱ��=ϵͳʱ��/(I2CPSC+1)/((I2CCLKL+6) +(I2CCLKH+6)) 
	//when I2CPSC =0   ʱ��=ϵͳʱ��/(I2CPSC+1)/((I2CCLKL+7) +(I2CCLKH+7)) 
	I2caRegs.I2CPSC.all = 6;								
	I2caRegs.I2CCLKL = 45;			
	I2caRegs.I2CCLKH = 70;			
	
	I2caRegs.I2CIER.all = 0x0000;								// ���ж�
	I2caRegs.I2CMDR.bit.IRS = 1;								// ʹ��I2Cģ��
	I2caRegs.I2CMDR.all = 0x0020;								//ʹ��I2C ģ��(ͬ��һ�������ͬ����)
	I2caRegs.I2CFFTX.all = 0x6000;								// ʹ�� FIFO �� TXFIFO
	I2caRegs.I2CFFRX.all = 0x2000;								// ʹ�� RXFIFO,�� RXFFINT, 
  
	I2caRegs.I2CCNT=0x0000;
	I2caRegs.I2CSTR.all=0xFFFF;								//������б�־λ
//I2Cʱ��ʹ�ܣ�Ҳ���԰Ѹ���������ϵͳʱ�ӳ�ʼ��
//	SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;   					// I2C

	for (i16Temp = 0;i16Temp < 10000;i16Temp++)				//�ȴ�10ms����ȷ�ж�����״̬
  	{
		asm( " rpt #99||nop");
  	}
}	


	
//===========================================================================
// End of file.
//===========================================================================
