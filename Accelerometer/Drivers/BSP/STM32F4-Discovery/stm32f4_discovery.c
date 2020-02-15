/**
  ******************************************************************************
  * @file    stm32f4_discovery.c
  * @author  MCD Application Team
  * @version V2.0.2
  * @date    26-June-2014
  * @brief   This file provides set of firmware functions to manage Leds and
  *          push-button available on STM32F4-Discovery Kit from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */   
    
/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL 
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on STM32F4-Discovery Kit from STMicroelectronics.
  * @{
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Defines
  * @{
  */
  
  /**
  * @brief STM32F4 DISCO BSP Driver version number V2.0.2
  */
#define __STM32F4_DISCO_BSP_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32F4_DISCO_BSP_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __STM32F4_DISCO_BSP_VERSION_SUB2   (0x02) /*!< [15:8]  sub2 version */
#define __STM32F4_DISCO_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __STM32F4_DISCO_BSP_VERSION         ((__STM32F4_DISCO_BSP_VERSION_MAIN << 24)\
                                             |(__STM32F4_DISCO_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32F4_DISCO_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32F4_DISCO_BSP_VERSION_RC)) 
/**
  * @}
  */ 


/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Variables
  * @{
  */ 
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED4_GPIO_PORT, 
                                 LED3_GPIO_PORT, 
                                 LED5_GPIO_PORT,
                                 LED6_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED4_PIN, 
                                 LED3_PIN, 
                                 LED5_PIN,
                                 LED6_PIN};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY_BUTTON_GPIO_PORT}; 
const uint16_t BUTTON_PIN[BUTTONn] = {KEY_BUTTON_PIN}; 
const uint8_t BUTTON_IRQn[BUTTONn] = {KEY_BUTTON_EXTI_IRQn};

uint32_t I2cxTimeout = I2Cx_TIMEOUT_MAX;    /*<! Value of Timeout when I2C communication fails */ 
uint32_t SpixTimeout = SPIx_TIMEOUT_MAX;    /*<! Value of Timeout when SPI communication fails */

static SPI_HandleTypeDef    SpiHandle;
/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Functions
  * @{
  */ 
static void     SPIx_Init(void);
static void     SPIx_MspInit(void);
static uint8_t  SPIx_WriteRead(uint8_t Byte);
static  void    SPIx_Error(void);

/* Link functions for Accelerometer peripheral */
void            ACCELERO_IO_Init(void);
void            ACCELERO_IO_ITConfig(void);
void            ACCELERO_IO_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void            ACCELERO_IO_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

/* Link functions for Audio peripheral */
void            AUDIO_IO_Init(void);
void            AUDIO_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t         AUDIO_IO_Read(uint8_t Addr, uint8_t Reg);
/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_LED_Functions
  * @{
  */ 

/**
  * @brief  This method returns the STM32F4 DISCO BSP Driver revision
  * @param  None
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32F4_DISCO_BSP_VERSION;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6
  * @retval None
  */
//void BSP_LED_Init(Led_TypeDef Led)
//{
//  GPIO_InitTypeDef  GPIO_InitStruct;
//
//  /* Enable the GPIO_LED Clock */
//  LEDx_GPIO_CLK_ENABLE(Led);
//
//  /* Configure the GPIO_LED pin */
//  GPIO_InitStruct.Pin = GPIO_PIN[Led];
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//
//  HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStruct);
//
//  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET);
//}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6  
  * @retval None
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET); 
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6 
  * @retval None
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET); 
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6  
  * @retval None
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_BUTTON_Functions
  * @{
  */ 

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_KEY
  * @param  Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability  
  * @retval None
  */
//void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Mode)
//{
//  GPIO_InitTypeDef GPIO_InitStruct;
//
//  /* Enable the BUTTON Clock */
//  BUTTONx_GPIO_CLK_ENABLE(Button);
//
//  if (Mode == BUTTON_MODE_GPIO)
//  {
//    /* Configure Button pin as input */
//    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//
//    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
//  }
//
//  if (Mode == BUTTON_MODE_EXTI)
//  {
//    /* Configure Button pin as input with External interrupt */
//    GPIO_InitStruct.Pin = BUTTON_PIN[Button];
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
//
//    /* Enable and set Button EXTI Interrupt to the lowest priority */
//    HAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[Button]), 0x0F, 0);
//    HAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
//  }
//}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter should be: BUTTON_KEY  
  * @retval The Button GPIO pin value.
  */
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_BUS_Functions
  * @{
  */ 

/*******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/

/******************************* SPI Routines *********************************/

/**
  * @brief  SPIx Bus initialization
  * @param  None
  * @retval None
  */
static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&SpiHandle) == HAL_SPI_STATE_RESET)
  {
    /* SPI configuration -----------------------------------------------------*/
    SpiHandle.Instance = DISCOVERY_SPIx;
    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    SpiHandle.Init.Direction = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    SpiHandle.Init.CRCPolynomial = 7;
    SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode = SPI_TIMODE_DISABLED;
    SpiHandle.Init.Mode = SPI_MODE_MASTER;

    SPIx_MspInit();
    HAL_SPI_Init(&SpiHandle);
  }
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte: Byte send.
  * @retval The received byte value
  */
static uint8_t SPIx_WriteRead(uint8_t Byte)
{
  uint8_t receivedbyte = 0;
  
  /* Send a Byte through the SPI peripheral */
  /* Read byte from the SPI bus */
  if(HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*) &Byte, (uint8_t*) &receivedbyte, 1, SpixTimeout) != HAL_OK)
  {
    SPIx_Error();
  }
  
  return receivedbyte;
}

/**
  * @brief  SPIx error treatment function.
  * @param  None
  * @retval None
  */
static void SPIx_Error(void)
{
  /* De-initialize the SPI comunication bus */
  HAL_SPI_DeInit(&SpiHandle);
  
  /* Re-Initiaize the SPI comunication bus */
  SPIx_Init();
}

/**
  * @brief  SPI MSP Init.
  * @param  hspi: SPI handle
  * @retval None
  */
static void SPIx_MspInit(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable the SPI peripheral */
  DISCOVERY_SPIx_CLK_ENABLE();
  
  /* Enable SCK, MOSI and MISO GPIO clocks */
  DISCOVERY_SPIx_GPIO_CLK_ENABLE();
  
  /* SPI SCK, MOSI, MISO pin configuration */
  GPIO_InitStructure.Pin = (DISCOVERY_SPIx_SCK_PIN | DISCOVERY_SPIx_MISO_PIN | DISCOVERY_SPIx_MOSI_PIN);
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = DISCOVERY_SPIx_AF;
  HAL_GPIO_Init(DISCOVERY_SPIx_GPIO_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/

/***************************** LINK ACCELEROMETER *****************************/

/**
  * @brief  Configures the Accelerometer SPI interface.
  * @param  None
  * @retval None
  */
void ACCELERO_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure the Accelerometer Control pins --------------------------------*/
  /* Enable CS GPIO clock and configure GPIO pin for Accelerometer Chip select */  
  ACCELERO_CS_GPIO_CLK_ENABLE();
  
  /* Configure GPIO PIN for LIS Chip select */
  GPIO_InitStructure.Pin = ACCELERO_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(ACCELERO_CS_GPIO_PORT, &GPIO_InitStructure);
  
  /* Deselect: Chip Select high */
  ACCELERO_CS_HIGH();
  
  SPIx_Init();
}

/**
  * @brief  Configures the Accelerometer INT2.
  *         EXTI0 is already used by user button so INT1 is not configured here.
  * @param  None
  * @retval None
  */
void ACCELERO_IO_ITConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable INT2 GPIO clock and configure GPIO PINs to detect Interrupts */
  ACCELERO_INT_GPIO_CLK_ENABLE();
  
  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.Pin = ACCELERO_INT2_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ACCELERO_INT_GPIO_PORT, &GPIO_InitStructure);
  
  /* Enable and set Accelerometer INT2 to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)ACCELERO_INT2_EXTI_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)ACCELERO_INT2_EXTI_IRQn);
}

/**
  * @brief  Writes one byte to the Accelerometer.
  * @param  pBuffer: pointer to the buffer containing the data to be written to the Accelerometer.
  * @param  WriteAddr: Accelerometer's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void ACCELERO_IO_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit: 
     - When 0, the address will remain unchanged in multiple read/write commands.
     - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  ACCELERO_CS_LOW();
  
  /* Send the Address of the indexed register */
  SPIx_WriteRead(WriteAddr);
  
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    SPIx_WriteRead(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  ACCELERO_CS_HIGH();
}

/**
  * @brief  Reads a block of data from the Accelerometer.
  * @param  pBuffer: pointer to the buffer that receives the data read from the Accelerometer.
  * @param  ReadAddr: Accelerometer's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the Accelerometer.
  * @retval None
  */
void ACCELERO_IO_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  ACCELERO_CS_LOW();
  
  /* Send the Address of the indexed register */
  SPIx_WriteRead(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to ACCELEROMETER (Slave device) */
    *pBuffer = SPIx_WriteRead(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  ACCELERO_CS_HIGH();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
