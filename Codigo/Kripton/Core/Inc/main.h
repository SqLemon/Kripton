/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INT_LED_Pin GPIO_PIN_13
#define INT_LED_GPIO_Port GPIOC
#define BTN1_Pin GPIO_PIN_14
#define BTN1_GPIO_Port GPIOC
#define BTN2_Pin GPIO_PIN_15
#define BTN2_GPIO_Port GPIOC
#define ENCL_A_Pin GPIO_PIN_0
#define ENCL_A_GPIO_Port GPIOA
#define ENCL_B_Pin GPIO_PIN_1
#define ENCL_B_GPIO_Port GPIOA
#define QXL_Pin GPIO_PIN_2
#define QXL_GPIO_Port GPIOA
#define CNY_Pin GPIO_PIN_3
#define CNY_GPIO_Port GPIOA
#define QXLF_Pin GPIO_PIN_4
#define QXLF_GPIO_Port GPIOA
#define QXRF_Pin GPIO_PIN_5
#define QXRF_GPIO_Port GPIOA
#define ENCR_A_Pin GPIO_PIN_6
#define ENCR_A_GPIO_Port GPIOA
#define ENCR_B_Pin GPIO_PIN_7
#define ENCR_B_GPIO_Port GPIOA
#define QXR_Pin GPIO_PIN_0
#define QXR_GPIO_Port GPIOB
#define BTN3_Pin GPIO_PIN_1
#define BTN3_GPIO_Port GPIOB
#define TX_BT_Pin GPIO_PIN_10
#define TX_BT_GPIO_Port GPIOB
#define RX_BT_Pin GPIO_PIN_11
#define RX_BT_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_12
#define LED3_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_13
#define LED2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOB
#define MOTR_DIR_Pin GPIO_PIN_15
#define MOTR_DIR_GPIO_Port GPIOB
#define MOTR_PWM_Pin GPIO_PIN_8
#define MOTR_PWM_GPIO_Port GPIOA
#define PROG_TX_Pin GPIO_PIN_9
#define PROG_TX_GPIO_Port GPIOA
#define PROG_RX_Pin GPIO_PIN_10
#define PROG_RX_GPIO_Port GPIOA
#define MOTL_PWM_Pin GPIO_PIN_11
#define MOTL_PWM_GPIO_Port GPIOA
#define MOTL_DIR_Pin GPIO_PIN_12
#define MOTL_DIR_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_15
#define BUZZER_GPIO_Port GPIOA
#define TX_SIDE_Pin GPIO_PIN_3
#define TX_SIDE_GPIO_Port GPIOB
#define TX_SIDE_H_Pin GPIO_PIN_4
#define TX_SIDE_H_GPIO_Port GPIOB
#define TXRF_Pin GPIO_PIN_5
#define TXRF_GPIO_Port GPIOB
#define TXLF_Pin GPIO_PIN_6
#define TXLF_GPIO_Port GPIOB
#define MPU_INT_Pin GPIO_PIN_7
#define MPU_INT_GPIO_Port GPIOB
#define SCL_MPU_Pin GPIO_PIN_8
#define SCL_MPU_GPIO_Port GPIOB
#define SDA_MPU_Pin GPIO_PIN_9
#define SDA_MPU_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
