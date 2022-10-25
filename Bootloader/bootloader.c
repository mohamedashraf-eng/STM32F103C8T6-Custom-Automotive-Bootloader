/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    bootloader.c
  * @brief  
	*
	******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "bootloader.h"
#include "usart.h"
#include "crc.h"

/* Macros ------------------------------------------------------------------*/


/* Data types ------------------------------------------------------------------*/
static uint8_t G_u8BtlHostBuffer[BTL_HOST_BUFFER_RX_SIZE];

/* Public Functions ------------------------------------------------------------------*/
EN_BTLStatus_t 
BTL_EnFetchHostCmd(void)
{
	/* Indicator message */
	BTL_voidPrintDbgMsg("Bootloader Application Started. \r\n");
	
	EN_BTLStatus_t L_EnThisFunctionStatus = BTL_NACK;
	HAL_StatusTypeDef L_EnHalUartStatus = HAL_ERROR;
	uint8_t L_u8DataLength = 0;
	
	/* Flush the buffer */
	memset(G_u8BtlHostBuffer, 0, BTL_HOST_BUFFER_RX_SIZE);
	
	/* Receive the data */
	L_EnHalUartStatus = HAL_UART_Receive(BTL_CHANNEL_UART, 
																			 G_u8BtlHostBuffer, 
																			 BTL_HOST_RX_CMD_SIZE, 
																			 HAL_MAX_DELAY);
	if( (HAL_OK != L_EnHalUartStatus) )
	{
		/* Error handler */
		BTL_voidPrintDbgMsg("Error! L_EnHalUartStatus: %d \r\n", L_EnHalUartStatus);
	}
	else
	{
		/* Get the data length */
		L_u8DataLength = G_u8BtlHostBuffer[0];
		
		/* Start receiving data */
		L_EnHalUartStatus = HAL_UART_Receive(BTL_CHANNEL_UART, 
																				 &G_u8BtlHostBuffer[1], 
																				 L_u8DataLength, 
																				 HAL_MAX_DELAY);
		if( (HAL_OK != L_EnHalUartStatus) )
		{
			/* Error handler */
			BTL_voidPrintDbgMsg("Error! L_EnHalUartStatus: %d \r\n", L_EnHalUartStatus);
		}
		else
		{
			/* Switch on the type of the command */
			switch(G_u8BtlHostBuffer[1])
			{
				case CBL_GET_VER_CMD						:
						BTL_voidPrintDbgMsg("CBL_GET_VER_CMD \r\n");
						/* Perform the functionality */
						BTL_voidGetVersion(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				case CBL_GET_HELP_CMD						:
						BTL_voidPrintDbgMsg("CBL_GET_HELP_CMD \r\n");
						/* Perform the functionality */
						BTL_voidGetHelp(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				case CBL_GET_CID_CMD						:
						BTL_voidPrintDbgMsg("CBL_GET_CID_CMD \r\n");
						/* Perform the functionality */
						BTL_voidGetChipIdNumber(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				case CBL_GET_RDP_STATUS_CMD			:
						BTL_voidPrintDbgMsg("CBL_GET_RDP_STATUS_CMD \r\n");
						/* Perform the functionality */
						BTL_voidGetSectorProtectionStatus(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				case CBL_GO_TO_ADDR_CMD					:
						BTL_voidPrintDbgMsg("CBL_GO_TO_ADDR_CMD \r\n");
						/* Perform the functionality */
						BTL_voidJumpToAddress(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				case CBL_FLASH_ERASE_CMD				:
						BTL_voidPrintDbgMsg("CBL_FLASH_ERASE_CMD \r\n");
						/* Perform the functionality */
						BTL_voidEraseFlash(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				case CBL_MEM_WRITE_CMD					:
						BTL_voidPrintDbgMsg("CBL_MEM_WRITE_CMD \r\n");
						/* Perform the functionality */
						BTL_voidMemoryWrite(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				case CBL_EN_R_W_PROTECT_CMD		  :
						BTL_voidPrintDbgMsg("CBL_EN_R_W_PROTECT_CMD \r\n");
						/* Perform the functionality */
						BTL_voidEnableRWProtection(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				case CBL_MEM_READ_CMD					  :
						BTL_voidPrintDbgMsg("CBL_MEM_READ_CMD \r\n");
						/* Perform the functionality */
						BTL_voidMemoryRead(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				case CBL_OTP_READ_CMD					  :
						BTL_voidPrintDbgMsg("CBL_OTP_READ_CMD \r\n");
						/* Perform the functionality */
						BTL_voidReadOTP(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				case CBL_DIS_R_W_PROTECT_CMD		:
						BTL_voidPrintDbgMsg("CBL_DIS_R_W_PROTECT_CMD \r\n");
						/* Perform the functionality */
						BTL_voidDisableRWProtection(G_u8BtlHostBuffer);
						/* Set the new status */
						L_EnHalUartStatus = BTL_ACK;
					break;
				default: 
						BTL_voidPrintDbgMsg("Invalid cmd code received from host !! \r\n");
					break;
			}/* End switch */
		}/* End else 2 */
	}/* End else 1 */

	return L_EnThisFunctionStatus;
}/** @end BTL_EnFetchHostCmd */

/* Private Functions ------------------------------------------------------------------*/

static void 
BTL_voidJumpToUserApp(void)
{
	/* Read the data stored in the first 4 bytes (Main Stack Pointer) */
	uint32_t L_u32MspValue = *((uint32_t volatile *)(FLASH_BASE_APP_ADDR));
	/* Read the next 4 bytes from the base address (Reset Handler Function) */
	uint32_t L_u32ResetHandler = *((uint32_t volatile *) (FLASH_BASE_APP_ADDR + 4U));
	
	/* Set the reset handler as function */
	void (*L_vAppResetFunc)(void) = (void *) L_u32ResetHandler;
	
	/* Set the MSP for the application */
	__set_MSP(L_u32MspValue);
	
	/* Reset all the modules before start */
	HAL_RCC_DeInit();
	
	/* Call the reset function to start the application */
	L_vAppResetFunc();
}/** @end BTL_voidJumpToUserApp */

static void 
BTL_voidSendAckToHost(uint8_t DataLength)
{
	uint8_t L_u8AckValue[2u] = {BTL_ACK, DataLength};
	
	BTL_voidSendDataToHost(L_u8AckValue, 2u);
}/** @end BTL_voidSendAckToHost */

static void 
BTL_voidSendNackToHost(void)
{
	uint8_t L_u8NackValue = BTL_NACK;
	
	BTL_voidSendDataToHost(&L_u8NackValue, 1u);
}/** @end BTL_voidSendNackToHost */

static void  
BTL_voidPrintDbgMsg(char *Address_u8Format, ...)
{
	uint8_t L_u8MessageArr[100] = {0};
	
	/* Create variadic argument */
	va_list args;
	/* Enable access to the variadic argument */
	va_start(args, Address_u8Format);
	/* Write formatted data from variable argument list to string */
	vsprintf((char *)L_u8MessageArr, Address_u8Format, args);
	/* Print the message via the uartx channel speicfied */
	HAL_UART_Transmit(BTL_DEBUG_UART, L_u8MessageArr, sizeof(L_u8MessageArr), HAL_MAX_DELAY);
	/* Clean up the instant */
	
	return;
	va_end(args);
}/** @end BTL_voidPrintDbgMsg */

static void 
BTL_voidSendDataToHost(uint8_t *Address_u8Data, uint8_t Copy_u8DataLength)
{
	HAL_UART_Transmit(BTL_CHANNEL_UART, Address_u8Data, Copy_u8DataLength, HAL_MAX_DELAY);
}/** @end BTL_voidSendDataToHost */

/** @defgroup Bootloader cmd functions */
static void
BTL_voidGetVersion(uint8_t *Address_u8RxHostBuffer)
{
	/* Define the identification message */
	uint8_t L_u8BtlData[4u] = 
	{
		BTL_VENDOR_ID, 
		BTL_SW_MAJOR_VERSION, 
		BTL_SW_MINOR_VERSION, 
		BTL_SW_PATCH_VERSION
	};
	
	/* Packet length = Data length bytes + The length byte */
	uint8_t  L_u8HostCmdPacketLength = Address_u8RxHostBuffer[0] + 1U;
	/* To get the 4 bytes CRC located at the last 4. Get the arr_sz-1 then - 4 */
	uint32_t L_u32HostCrc32 = 
	*((uint32_t *) ((Address_u8RxHostBuffer + L_u8HostCmdPacketLength) - CRC_LENGTH_IN_BYTES));
	
	/* CRC Verification */
	EN_BtlCRC32Status_t L_EnCRC32VerifyStatus =
	BTL_EnVerificateCRC32(Address_u8RxHostBuffer, (L_u8HostCmdPacketLength-CRC_LENGTH_IN_BYTES), L_u32HostCrc32);
	
	if( (BTL_CRC32_NACK == L_EnCRC32VerifyStatus) )	
	{
		/* Error handle */
		BTL_voidSendNackToHost();
		BTL_voidPrintDbgMsg("Error! BTL_voidGetVersion: CRC32 not valid \r\n");
	}
	else 
	{
		BTL_voidSendAckToHost(4u);
		BTL_voidSendDataToHost(L_u8BtlData, 4u);
	}
		
}/** @end BTL_voidGetVersion */
static void
BTL_voidGetHelp(uint8_t *Address_u8RxHostBuffer)
{
	const uint8_t L_u8BtlHelpCmd[NUM_OF_CMD] = 
	{
		NUM_OF_CMD,
		CBL_GET_VER_CMD,				
		CBL_GET_HELP_CMD,			
		CBL_GET_CID_CMD,				
		CBL_GET_RDP_STATUS_CMD,
		CBL_GO_TO_ADDR_CMD,
		CBL_FLASH_ERASE_CMD,		
		CBL_MEM_WRITE_CMD,			
		CBL_EN_R_W_PROTECT_CMD,
		CBL_MEM_READ_CMD,			
		CBL_OTP_READ_CMD,			
		CBL_DIS_R_W_PROTECT_CMD
	};
	
	/* Packet length = Data length bytes + The length byte */
	uint8_t  L_u8HostCmdPacketLength = Address_u8RxHostBuffer[0] + 1U;
	/* To get the 4 bytes CRC located at the last 4. Get the arr_sz-1 then - 4 */
	uint32_t L_u32HostCrc32 = 
	*((uint32_t *) ((Address_u8RxHostBuffer + L_u8HostCmdPacketLength) - CRC_LENGTH_IN_BYTES));
	
	/* CRC Verification */
	EN_BtlCRC32Status_t L_EnCRC32VerifyStatus =
	BTL_EnVerificateCRC32(Address_u8RxHostBuffer,
											  (L_u8HostCmdPacketLength-CRC_LENGTH_IN_BYTES),
												L_u32HostCrc32);
	
	if( (BTL_CRC32_NACK == L_EnCRC32VerifyStatus) )
	{
		/* Error handle */
		BTL_voidSendNackToHost();
		BTL_voidPrintDbgMsg("Error! BTL_voidGetHelp: CRC32 not valid \r\n");
	}
	else 
	{
		BTL_voidSendAckToHost(NUM_OF_CMD);
		BTL_voidSendDataToHost(L_u8BtlHelpCmd, NUM_OF_CMD);
	}
	
}/** @end BTL_voidGetHelp */
static void 
BTL_voidGetChipIdNumber(uint8_t *Address_u8RxHostBuffer)
{
	/* Packet length = Data length bytes + The length byte */
	uint8_t  L_u8HostCmdPacketLength = Address_u8RxHostBuffer[0] + 1U;
	/* To get the 4 bytes CRC located at the last 4. Get the arr_sz-1 then - 4 */
	uint32_t L_u32HostCrc32 = 
	*((uint32_t *) ((Address_u8RxHostBuffer + L_u8HostCmdPacketLength) - CRC_LENGTH_IN_BYTES));
	
	/* CRC Verification */
	EN_BtlCRC32Status_t L_EnCRC32VerifyStatus =
	BTL_EnVerificateCRC32(Address_u8RxHostBuffer, (L_u8HostCmdPacketLength-CRC_LENGTH_IN_BYTES), L_u32HostCrc32);
	
	if( (BTL_CRC32_NACK == L_EnCRC32VerifyStatus) )
	{
		/* Error handle */
		BTL_voidSendNackToHost();
		BTL_voidPrintDbgMsg("Error! BTL_voidGetHelp: CRC32 not valid \r\n");
	}
	else 
	{
		/* Extract the chip id from the MCU vendor support register */
		uint16_t L_u16ChipId = (uint16_t)((DBGMCU->IDCODE) & (0x00000FFF)); 

		BTL_voidSendAckToHost(2u);
		BTL_voidSendDataToHost((uint8_t *)&L_u16ChipId, 2u);
	}
	
	BTL_voidJumpToUserApp();
}/** @end BTL_voidGetChipIdNumber */
static void
BTL_voidJumpToAddress(uint8_t *Address_u8RxHostBuffer)
{
	/* Packet length = Data length bytes + The length byte */
	uint8_t  L_u8HostCmdPacketLength = Address_u8RxHostBuffer[0] + 1U;
	/* To get the 4 bytes CRC located at the last 4. Get the arr_sz-1 then - 4 */
	uint32_t L_u32HostCrc32 =
	*((uint32_t *) ((Address_u8RxHostBuffer + L_u8HostCmdPacketLength) - CRC_LENGTH_IN_BYTES));

	uint32_t L_u32HostJmpAddr = 0;

	/* CRC Verification */
	EN_BtlCRC32Status_t L_EnCRC32VerifyStatus =
	BTL_EnVerificateCRC32(Address_u8RxHostBuffer, (L_u8HostCmdPacketLength-CRC_LENGTH_IN_BYTES), L_u32HostCrc32);

	if( (BTL_CRC32_NACK == L_EnCRC32VerifyStatus) )
	{
		/* Error handle */
		BTL_voidSendNackToHost();
		BTL_voidPrintDbgMsg("Error! BTL_voidGetVersion: CRC32 not valid \r\n");
	}
	else
	{
		BTL_voidSendAckToHost(1u);

		/* Extract the address from the host msg */
		L_u32HostJmpAddr = *((uint32_t *) &Address_u8RxHostBuffer[2]);
		/* Address verification */
		EN_BtlHostJmpAddrStatus_t L_EnAddrVerifyStatus = BTL_EnVerifyHostJmpAddr(L_u32HostJmpAddr);

		if( (BTL_HOST_JMP_ADDR_ACK == L_EnAddrVerifyStatus) )
		{
			/* Notify the host */
			BTL_voidPrintDbgMsg("Host Jump Address Valid: 0x%x\r\n", L_u32HostJmpAddr);
			BTL_voidSendDataToHost(&L_EnCRC32VerifyStatus, 1u);
			/* L_u32HostJmpAddr + 1 : For ARM-Thumb ISA indication */
			void (*L_vAddrToJmp)(void) = (void *) (L_u32HostJmpAddr + 1U);
			/* Jump to the specified address */
			L_vAddrToJmp();
		}
		else
		{
			/* Notify the host */
			BTL_voidPrintDbgMsg("Host Jump Address Invalid: 0x%x\r\n", L_u32HostJmpAddr);
			BTL_voidSendDataToHost(&L_EnCRC32VerifyStatus, 1u);
		}
	}
}/** @end BTL_voidJumpToAddress */
static void
BTL_voidGetSectorProtectionStatus(uint8_t *Address_u8RxHostBuffer)
{

}/** @end BTL_voidGetSectorProtectionStatus */
static void
BTL_voidEraseFlash(uint8_t *Address_u8RxHostBuffer)
{

}/** @end BTL_voidEraseFlash */
static void
BTL_voidMemoryWrite(uint8_t *Address_u8RxHostBuffer)
{

}/** @end BTL_voidMemoryWrite */
static void
BTL_voidEnableRWProtection(uint8_t *Address_u8RxHostBuffer)
{

}/** @end BTL_voidEnableRWProtection */
static void
BTL_voidMemoryRead(uint8_t *Address_u8RxHostBuffer)
{

}/** @end BTL_voidMemoryRead */
static void
BTL_voidReadOTP(uint8_t *Address_u8RxHostBuffer)
{

}/** @end BTL_voidReadOTP */
static void
BTL_voidDisableRWProtection(uint8_t *Address_u8RxHostBuffer)
{

}/** @end BTL_voidDisableRWProtection */

static EN_BtlHostJmpAddrStatus_t
BTL_EnVerifyHostJmpAddr(uint32_t Copy_u32HostJmpAddr)
{
	EN_BtlHostJmpAddrStatus_t L_EnFunctionState = BTL_HOST_JMP_ADDR_NACK;

	/* Check the RAM Address Range */
	if( (Copy_u32HostJmpAddr >= SRAM_BASE) &&
		  (Copy_u32HostJmpAddr <= STM32F103C8Tx_SRAM1_END) )
	{
		L_EnFunctionState = BTL_HOST_JMP_ADDR_ACK;
	}
	/* -> STM32F103C8T6 Only 1 SRAM
	else if( (Copy_u32HostJmpAddr >= SRAM2_BASE) &&
		  		 (Copy_u32HostJmpAddr <= STM32F103C8Tx_SRAM2_END) )
	{
		L_EnFunctionState = BTL_HOST_JMP_ADDR_ACK;
	}
	else if( (Copy_u32HostJmpAddr >= SRAM3_BASE) &&
		  		 (Copy_u32HostJmpAddr <= STM32F103C8Tx_SRAM3_END) )
	{
		L_EnFunctionState = BTL_HOST_JMP_ADDR_ACK;
	}
	*/
	/* Check the FLASH Address Range */
	else if( (Copy_u32HostJmpAddr >= FLASH_BASE) &&
		  		 (Copy_u32HostJmpAddr <= STM32F103C8Tx_FLASH_END) )
	{
		L_EnFunctionState = BTL_HOST_JMP_ADDR_ACK;
	}
	else
	{
		/* Error handle */
	}

	return L_EnFunctionState;
}/** @end BTL_EnVerifyHostJmpAddr */

static EN_BtlCRC32Status_t
BTL_EnVerificateCRC32(uint8_t *Address_u8RxHostBuffer, 
											uint16_t Copy_u16DataLength, 
											uint32_t Copy_u32CalculatedCRC32)
{
	EN_BtlCRC32Status_t L_EnCRC32VerifyStatus = BTL_CRC32_NACK;
	
	uint32_t MCU_CRC_Calculated = 0;
	uint8_t Data_Counter = 0;
	uint32_t Data_Buffer = 0;
	
	/* Calculate CRC32 */
	for(Data_Counter = 0; Data_Counter < Copy_u16DataLength; Data_Counter++)
	{
		Data_Buffer = (uint32_t)Address_u8RxHostBuffer[Data_Counter];
		MCU_CRC_Calculated = HAL_CRC_Accumulate(BTL_CRC_ENGINE, &Data_Buffer, 1);
	}

	/* Reset the CRC Calculation Unit */
  __HAL_CRC_DR_RESET(BTL_CRC_ENGINE);
	
	/* Compare the Host CRC and Calculated CRC */
	if(MCU_CRC_Calculated == Copy_u32CalculatedCRC32)
	{
		L_EnCRC32VerifyStatus = BTL_CRC32_ACK;
	}
	else{
		BTL_voidPrintDbgMsg("Error! BTL_EnVerificateCRC32: Verification Failed.\r\n");
	}
	
	return L_EnCRC32VerifyStatus;
}/** @end BTL_EnVerificateCRC32 */





