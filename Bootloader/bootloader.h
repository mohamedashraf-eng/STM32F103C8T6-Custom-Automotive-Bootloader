/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    bootloader.h
  * @brief   
	*
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

/* Includes ------------------------------------------------------------------*/



/* Macros ------------------------------------------------------------------*/
#define BTL_CHANNEL_UART 				(&huart1)
#define BTL_DEBUG_UART   				(&huart2)
#define BTL_CRC_ENGINE					(&hcrc)
#define BTL_HOST_RX_CMD_SIZE 		(1U)
#define BTL_HOST_BUFFER_RX_SIZE (200U)

/** @defgroup Bootloader commands */
#define NUM_OF_CMD 								(0x0CU)
#define	CBL_GET_VER_CMD						(0x10U)
#define	CBL_GET_HELP_CMD					(0x11U)
#define	CBL_GET_CID_CMD						(0x12U)
#define	CBL_GET_RDP_STATUS_CMD		(0x13U)
#define	CBL_GO_TO_ADDR_CMD				(0x14U)
#define	CBL_FLASH_ERASE_CMD				(0x15U)
#define	CBL_MEM_WRITE_CMD					(0x16U)
#define	CBL_EN_R_W_PROTECT_CMD		(0x17U)
#define	CBL_MEM_READ_CMD					(0x18U)
#define	CBL_OTP_READ_CMD					(0x19U)
#define	CBL_DIS_R_W_PROTECT_CMD		(0x1AU)
#define CBL_READ_SECTOR_STATUS 		(0x1BU)

/** @defgroup Bootloader virtual identifications */
#define BTL_VENDOR_ID 						(69U)
#define BTL_SW_MAJOR_VERSION			(1U)
#define BTL_SW_MINOR_VERSION			(0U)
#define BTL_SW_PATCH_VERSION			(0U)

/** @defgroup Packet informations */
#define CRC_LENGTH_IN_BYTES 			(4U)

/** @defgroup Application Code information */
#define FLASH_BASE_APP_ADDR 			(0x08003800U)

/** @defgroup Valid Jump Addresses Information */
#define STM32F103C8Tx_SRAM1_SIZE 	(1024U * 20U)
#define STM32F103C8Tx_SRAM1_END		(SRAM_BASE + STM32F103C8Tx_SRAM1_SIZE)
//#define STM32F103C8Tx_SRAM2_SIZE 	(1024U * )
//#define STM32F103C8Tx_SRAM2_END 	(SRAM2_BASE + STM32F103C8Tx_SRAM2_SIZE)
//#define STM32F103C8Tx_SRAM3_SIZE 	(1024U * )
//#define STM32F103C8Tx_SRAM3_END 	(CCMDATARAM_BASE + STM32F103C8Tx_SRAM3_SIZE)
#define STM32F103C8Tx_FLASH_SIZE 	(1024U * 64U)
#define STM32F103C8Tx_FLASH_END 	(FLASH_BASE + STM32F103C8Tx_FLASH_SIZE)

/* Data types ------------------------------------------------------------------*/

typedef enum EN_BTLStatus_t
{
	BTL_NACK = 0xAB,
	BTL_ACK  = 0xCD
}EN_BTLStatus_t;

typedef enum EN_BtlCRC32Status_t
{
	BTL_CRC32_NACK = 0x00U,
	BTL_CRC32_ACK  = 0x01U
}EN_BtlCRC32Status_t;

typedef enum EN_BtlHostJmpAddrStatus_t
{
	BTL_HOST_JMP_ADDR_NACK,
	BTL_HOST_JMP_ADDR_ACK
}EN_BtlHostJmpAddrStatus_t;

/* Public Functions ------------------------------------------------------------------*/

EN_BTLStatus_t 
BTL_EnFetchHostCmd(void);

static EN_BtlHostJmpAddrStatus_t
BTL_EnVerifyHostJmpAddr(uint32_t Copy_u32HostJmpAddr);

/* Private Functions ------------------------------------------------------------------*/
static void 
BTL_voidPrintDbgMsg(char *Address_u8Format, ...);

static void 
BTL_voidSendDataToHost(uint8_t *Address_u8Data, uint8_t Copy_u8DataLength);

static void 
BTL_voidSendNackToHost(void);

static void 
BTL_voidJumpToUserApp(void);

/** @defgroup Bootloader cmd functions */
static void
BTL_voidGetVersion(uint8_t *Address_u8RxHostBuffer);
static void
BTL_voidGetHelp(uint8_t *Address_u8RxHostBuffer);
static void
BTL_voidGetChipIdNumber(uint8_t *Address_u8RxHostBuffer);
static void
BTL_voidGetSectorProtectionStatus(uint8_t *Address_u8RxHostBuffer);
static void
BTL_voidJumpToAddress(uint8_t *Address_u8RxHostBuffer);
static void
BTL_voidEraseFlash(uint8_t *Address_u8RxHostBuffer);
static void
BTL_voidMemoryWrite(uint8_t *Address_u8RxHostBuffer);
static void
BTL_voidEnableRWProtection(uint8_t *Address_u8RxHostBuffer);
static void
BTL_voidMemoryRead(uint8_t *Address_u8RxHostBuffer);
static void
BTL_voidReadOTP(uint8_t *Address_u8RxHostBuffer);
static void
BTL_voidDisableRWProtection(uint8_t *Address_u8RxHostBuffer);


static EN_BtlCRC32Status_t
BTL_EnVerificateCRC32(uint8_t *Address_u8RxHostBuffer, 
											uint16_t Copy_u16DataLength, 
											uint32_t Copy_u32CalculatedCRC32);

#endif /* __BOOTLOADER_H__ */







