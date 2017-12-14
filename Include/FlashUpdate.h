//###########################################################################
//
// FILE:  FlashUpdate.h	
//
// TITLE: FlashUpdate include file
//
// DESCRIPTION:
//
// Function prototypes and macros for the FlashUpdate include file.
//
//###########################################################################
// Date:June 22, 2013 $
//###########################################################################


#ifndef FLASHUPDATE_2803x_H
#define FLASHUPDATE_2803x_H

#include "Flash2803x_API_Library.h"

#ifndef NULL
    #define NULL ((void * )0)
#endif

#define Drv_DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)

#define BOOTSWV	101
//status
#define	KERNEL_READY_FOR_ERASE		0x6C
#define	KERNEL_READY_FOR_DATA_RX	0x6E
#define	KERNEL_FLASH_NOT_ERASED		0x70
#define	KERNEL_PROGRAM_COMPLETE		5
#define	KERNEL_BLOCK_HEAD_OK		0
#define	KERNEL_BLOCK_SIZE_ERROR		10
#define	KERNEL_BLOCK_ADDR_ERROR		20
#define	KERNEL_DATA_CHKSUM_OK		0x82
#define	KERNEL_DATA_CHKSUM_ERROR	0x83
#define	KERNEL_ERASE_ALLFLASH		4

/*---------------------------------------------------------------------------
  Data buffer used for serial flash programming:  
---------------------------------------------------------------------------*/
#define  	FLASH_BUFFER_SIZE				0x200               // 512 Words

//Modify the follow keys for your specific firmware protection                      
#define PRG_Key0  	0xFFFF  	//PSWD bits 15-0
#define PRG_Key1  	0xFFFF  	//PSWD bits 31-16
#define PRG_Key2  	0xFFFF  	//PSWD bits 47-32
#define PRG_Key3  	0xFFFF  	//PSWD bits 63-48
#define PRG_Key4  	0xFFFF  	//PSWD bits 79-64
#define PRG_Key5  	0xFFFF 		//PSWD bits 95-80
#define PRG_Key6  	0xFFFF  	//PSWD bits 111-96 
#define PRG_Key7  	0xFFFF  	//PSWD bits 127-112

//��Ϣ֡����
#define	FRAME_UPDATECOMMAND			0x80
#define	FRAME_HANDSHAKE 			0x81
#define	FRAME_CSMUNLOCKSTATUS 		0x82
#define	FRAME_APIVERSIONSTATUS 		0x83
#define	FRAME_ERASE 				0x84
#define	FRAME_PROGRAM 				0x85
#define	FRAME_BLOCKHEAD 			0x86
#define	FRAME_BLOCKDATA 			0x87
#define	FRAME_CHECKSUM 				0x88
#define	FRAME_PROGRAMSTATUS 		0x89
#define	FRAME_VERIFYSTATUS 			0x8A
#define	FRAME_ERROR 				0x90

#define CMD00_FLASHUD				0x00
#define ID_FLASHUDRD				0x780
#define ID_FLASHUDWT				0x781

//For destination address validation
#define FLASH_START_ADDR  			0x3E8000
#define FLASH_END_ADDR    			0x3F7FFF
#define	FLASH_PROGRAM_START			0x3E8000
#define	FLASH_PROGRAM_END			0x3F5FFB

struct StructFlashUpdateFlag
{
	UINT16	bRecValid		:1;			// BIT0 ���ݽ�����Ч
	UINT16	bFlashErased	:1;			// BIT2 FlashErase���	
	UINT16	bReserved		:14;		// BIT2-BIT15 
};

struct StructFlashVariable
{	INT16	m_i16_FlashClockMissing;	//ʱ�Ӷ�ʧ��־λ
	UINT16	m_u16_RamCheckErr;			// RAM�Լ�����־
	UINT16 	u16_FrameType;				// ֡����
	UINT16	u16_EraseType;				// ��������:4-ȫ��;3-��BCD
	UINT16	u16_BlockSize;
	UINT16	u16_APIVersion;		
	UINT16	u16_BlockData[3];			// ÿ֡���յ�����Ч����
	UINT16	u16_MailBoxDLC;				// ÿ֡���ݵ�����(��CommandId)
	UINT16	u16_HandShakeData;			// CANͨѶ��������
	UINT16	u16_BlockSeqNum;			// ���Block������
	UINT32	u32_BlockAddr;
	UINT32	u32_CheckSumRec;			// ����У���
	UINT32	u32_CheckSumDSP;			// DSP�����У���	
	struct StructFlashUpdateFlag  st_FlashUpdateFlag;
	UINT16 *pDestBuffer;
	UINT16* p_SourceAddr;
	UINT16* p_SourceEndAddr; 
	UINT16* p_DestAddr;  	
	UINT16* p_Flash;	
	UINT16  FlashBuffer1[2];		// д�����֧��־������
	FLASH_ST 	FlashStatus1;  			// program api��������
	FLASH_ST 	FlashStatus;
	UINT16 u16_Status;
	UINT16 u16_MdlAddr;
	UINT16 u16_ModuleID;
};

extern UINT16 Flash28_API_LoadStart;
extern UINT16 Flash28_API_LoadEnd;
extern UINT16 Flash28_API_RunStart;

//============================================================
//	Class_FlashUpdate���ⲿ�Ľӿ�
//	1)
//	2)
//	3)����
//	4)����
//============================================================
void Drv_ExtOscSel (void);
void Drv_IntOsc1Sel (void);
void Drv_InitPll(Uint16 val, Uint16 divsel);
void Drv_InitSysCtrl(void);
void Drv_SPIInit(void);
void Drv_GPIOInit(void);
void Drv_PWMInit(void);
void Drv_eCANAInit(void);
void Drv_SystemInit(void);
void Drv_RamCheck(void);

void App_FlashAPIInit(void);
void App_FlashUpdateMain(void);		//flashUpdate ������
UINT16	App_Flash_CsmUnlock(void);	// ���ܺ���

void App_Delay1ms (void);
void App_Delay20ms (void);		
void App_MBox_Read(void);			// ��ȡ��������
void App_MBox_Send(UINT16 t_u16MsgType, UINT16 u16_Data);  //��������
void App_DSPRestart(void);			// dsp����
		
void MainSwitch(void);				//���г������
extern void DSP28x_usDelay(Uint32 Count);

#endif 
