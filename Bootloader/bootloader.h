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
#define BTL_CHANNEL_UART 							(&huart1)
#define BTL_DEBUG_UART   							(&huart2)
#define BTL_CRC_ENGINE								(&hcrc)
#define BTL_HOST_RX_CMD_SIZE 					(1U)
#define BTL_HOST_BUFFER_RX_SIZE 			(200U)

/** @defgroup Bootloader commands */
#define NUM_OF_CMD 										(0x0CU)
#define	CBL_GET_VER_CMD								(0x10U)
#define	CBL_GET_HELP_CMD							(0x11U)
#define	CBL_GET_CID_CMD								(0x12U)
#define	CBL_GET_RDP_STATUS_CMD				(0x13U)
#define	CBL_GO_TO_ADDR_CMD						(0x14U)
#define	CBL_FLASH_ERASE_CMD						(0x15U)
#define	CBL_MEM_WRITE_CMD							(0x16U)
#define	CBL_EN_R_W_PROTECT_CMD				(0x17U)
#define	CBL_MEM_READ_CMD							(0x18U)
#define	CBL_OTP_READ_CMD							(0x19U)
#define	CBL_DIS_R_W_PROTECT_CMD				(0x1AU)
#define CBL_READ_SECTOR_STATUS 				(0x1BU)

/** @defgroup Packet informations */
#define CRC_LENGTH_IN_BYTES 					(4u)

/** @defgroup Bootloader virtual identifications */
#define BTL_VENDOR_ID 								(69u)
#define BTL_SW_MAJOR_VERSION					(1u)
#define BTL_SW_MINOR_VERSION					(0u)
#define BTL_SW_PATCH_VERSION					(0u)

/** @defgroup Application Code information */
#define FLASH_BASE_APP_ADDR 					(0x08005000U)

/** @defgroup Valid Jump Addresses Information */
#define STM32F103C8Tx_SRAM_PAGE_SIZE 	(1024u)
#define STM32F103C8Tx_SRAM_PAGE_NUM 	(20u)
#define STM32F103C8Tx_SRAM1_SIZE 			(STM32F103C8Tx_SRAM_PAGE_SIZE * STM32F103C8Tx_SRAM_PAGE_NUM)
#define STM32F103C8Tx_SRAM1_END				(SRAM_BASE + STM32F103C8Tx_SRAM1_SIZE)
//#define STM32F103C8Tx_SRAM2_SIZE 		(1024U * )
//#define STM32F103C8Tx_SRAM2_END 		(SRAM2_BASE + STM32F103C8Tx_SRAM2_SIZE)
//#define STM32F103C8Tx_SRAM3_SIZE 		(1024U * )
//#define STM32F103C8Tx_SRAM3_END 		(CCMDATARAM_BASE + STM32F103C8Tx_SRAM3_SIZE)
#define STM32F103C8Tx_FLASH_PAGE_NUM 	(64u)
#define STM32F103C8Tx_FLASH_PAGE_SIZE (1024u)
#define STM32F103C8Tx_FLASH_SIZE 			(STM32F103C8Tx_FLASH_PAGE_SIZE * STM32F103C8Tx_FLASH_PAGE_NUM)
#define STM32F103C8Tx_FLASH_END 			(FLASH_BASE + STM32F103C8Tx_FLASH_SIZE)

/** @defgroup Flash Erasing Information */
#define BTL_FLASH_MAX_PAGE_NUM 				(STM32F103C8Tx_FLASH_PAGE_NUM)
#define BTL_FLASH_MASS_ERASE 					(BTL_FLASH_MAX_PAGE_NUM + 1u)

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
	BTL_HOST_JMP_ADDR_NACK = 0x00U,
	BTL_HOST_JMP_ADDR_ACK  = 0x01U
}EN_BtlHostJmpAddrStatus_t;

typedef enum EN_BtlFlashEraseStatus_t
{
	BTL_FLASH_ERASE_NACK = 0x00U,
	BTL_FLASH_ERASE_ACK  = 0x01U
}EN_BtlFlashEraseStatus_t;

typedef enum EN_BtlHostFlashWriteAddrStatus_t
{
	BTL_HOST_FLASH_ADDR_NACK = 0x00U,
	BTL_HOST_FLASH_ADDR_ACK  = 0x01U
}EN_BtlHostFlashWriteAddrStatus_t;

typedef enum EN_BtlRDPStatus_t
{
	BTL_RDP_NACK = 0x00U,
	BTL_RDP_ACK  = 0x01U
}EN_BtlRDPStatus_t;

/* Public Functions ------------------------------------------------------------------*/

EN_BTLStatus_t 
BTL_EnFetchHostCmd(void);

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
BTL_voidGetMemProtectionStatus(uint8_t *Address_u8RxHostBuffer);
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

static EN_BtlFlashEraseStatus_t
BTL_EnEraseFlashPages(uint8_t Copy_u8PageIndex,
											uint8_t Copy_u8NumOfPages);

static EN_BtlHostJmpAddrStatus_t
BTL_EnVerifyHostJmpAddr(uint32_t Copy_u32HostJmpAddr);

static uint32_t
BTL_u32PageIndexToPagedAddr(uint8_t Copy_u8PageIndex);

static EN_BtlHostFlashWriteAddrStatus_t
BTL_EnBtlWriteToAddr(uint8_t *Copy_u8HostPayload,
										 uint32_t Copy_u32PayloadBaseAddress,
										 uint16_t Copy_u16PayloadLength);

static EN_BtlRDPStatus_t
BTL_EnGetSTM32F103C8T6_ProtectionLvl(uint8_t *Address_u8TargetRDP);

#endif /* __BOOTLOADER_H__ */







