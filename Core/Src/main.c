/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LCD.h"
#include "RTC.h"
#include "keypad.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIME_DIGITS_LENGTH 6
#define DATE_DIGITS_LENGTH 6
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

ETH_HandleTypeDef heth;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
static const tick_t ADJUST_TIMEOUT = 60000; // wait a minute
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
static time_adjust_t get_time_from_keypad(datetime_data_t *);
static void display_datetime(datetime_data_t*);
static bool_t display_config_datetime(datetime_data_t *datetime, uint8_t key);
static void move_cursor_backward(uint8_t *cursor_x, uint8_t *cursor_y, size_t max_positions);
static void move_cursor_forward(uint8_t *cursor_x, uint8_t *cursor_y, size_t max_positions);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  keyboard_hw_t keyboard_hw = {
    {GPIOF, GPIO_PIN_13}, // L1
    {GPIOE, GPIO_PIN_9},  // L2
    {GPIOE, GPIO_PIN_11}, // L3
    {GPIOF, GPIO_PIN_14}, // L4
    {GPIOE, GPIO_PIN_13}, // C1
    {GPIOF, GPIO_PIN_15}, // C2
    {GPIOG, GPIO_PIN_14}, // C3
    {GPIOG, GPIO_PIN_9}   // C4
  };

  keypad_init(keyboard_hw);
  LCD_init(&hi2c1);
  RTC_init(&hi2c1);
  // FSM Initialization
  enum state_t { READ_TIME, DISPLAY_TIME, WAIT_FOR_INPUT, ADJUST_TIME, SAVE_TIME } state = READ_TIME;
  uint8_t key_pressed = '\0';
  datetime_data_t current_datetime = { 0 };
  time_adjust_t adjust_state = TIMEOUT_REACHED;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  keypad_update();
	  /*uint8_t character = keypad_get_key();
    //datetime_data_t timestamp = { 0 };
    //RTC_read(&timestamp);
	  uint8_t buffer [100] = {0};
	  sprintf((char*)buffer,"Tecla pulsada: %c",(char)character);
    if (character != '\0'){
      LCD_clear();
      LCD_print_string(buffer);
    }*/
	  //LCD_show_cursos_blink();
	  //LCD_shift_cursor('C', 'L');
	  //LCD_set_position_xy (5, 1);
    switch (state) {
      case READ_TIME:
         RTC_read(&current_datetime);
         state = DISPLAY_TIME;
         break;
      case DISPLAY_TIME:
          display_datetime(&current_datetime);
          state = WAIT_FOR_INPUT;
          break;
      case WAIT_FOR_INPUT:
          key_pressed = keypad_get_key();
          state = key_pressed == 'A' ? ADJUST_TIME : READ_TIME;
          break;
      case ADJUST_TIME:
          // trae la hora desde el teclado- mestra la pantalla de ajuste
          adjust_state = get_time_from_keypad(&current_datetime);
          if (adjust_state == READY_FOR_SAVE)
            state = SAVE_TIME;
          else if (adjust_state == ADJUSTING)
            state = ADJUST_TIME;
          else 
            state = READ_TIME;
          break;
      case SAVE_TIME:
          RTC_set(&current_datetime);
          state = READ_TIME;
          break;
      default:
          state = READ_TIME;
          break;
    }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PF13 PF14 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PF15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PE9 PE11 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PG9 PG14 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
 * @brief Displays current time and date on the LCD.
 * 
 * Shows the time on the first line and the date with the day of the week on the second line.
 * 
 * @param datetime Pointer to datetime_data_t containing the current time and date.
 * @retval none
 */
static void display_datetime(datetime_data_t *datetime)
{
  if(datetime == NULL) Error_Handler();

	uint8_t week [][7] = {
			"Dom",
			"Lun",
			"Mar",
			"Mie",
			"Jue",
			"Vie",
			"Sab"
	};
    uint8_t buffer[20];

    LCD_set_position_xy(0, 0);
    sprintf((char*)buffer, "%02d:%02d:%02d", datetime->time.hours, datetime->time.minutes, datetime->time.seconds);
    LCD_print_string(buffer);

    LCD_set_position_xy(0, 1);
    sprintf((char*)buffer, "%s %02d/%02d/%02d", week[datetime->date.day - 1], datetime->date.date, datetime->date.month, datetime->date.year);
    LCD_print_string(buffer);
}


/**
 * @brief Moves the cursor forward on the display.
 * 
 * Adjusts cursor_x and cursor_y to move forward, wrapping to the next row if needed.
 * 
 * @param cursor_x Pointer to horizontal cursor position.
 * @param cursor_y Pointer to vertical cursor position.
 * @param max_positions Maximum number of positions for cursor_x.
 * @retval none
 */
static void move_cursor_forward(uint8_t *cursor_x, uint8_t *cursor_y, size_t max_positions)
{
  if(cursor_x == NULL || cursor_y == NULL || max_positions <= 0) Error_Handler();

    if ((*cursor_x + 1) >= max_positions) {
        *cursor_y = (*cursor_y + 1) % 2;
    }
    *cursor_x = (*cursor_x + 1) % max_positions;
}

/**
 * @brief Moves the cursor backward on the display.
 * 
 * Adjusts cursor_x and cursor_y to move backward, wrapping to the previous row if needed.
 * 
 * @param cursor_x Pointer to horizontal cursor position.
 * @param cursor_y Pointer to vertical cursor position.
 * @param max_positions Maximum number of positions for cursor_x.
 * @retval none
 */
static void move_cursor_backward(uint8_t *cursor_x, uint8_t *cursor_y, size_t max_positions)
{
  if(cursor_x == NULL || cursor_y == NULL || max_positions <= 0) Error_Handler();

    *cursor_x = (*cursor_x == 0) ? (max_positions - 1) : (*cursor_x - 1);
    if (*cursor_x == (max_positions - 1) && *cursor_y == 0) {
        *cursor_y = 1;
    } else if (*cursor_x == (max_positions - 1) && *cursor_y == 1) {
        *cursor_y = 0;
    }
}


/**
 * @brief Handles time and date adjustment on the LCD.
 * 
 * Allows navigation and digit entry using keypad inputs, updating the datetime structure.
 * 
 * @param datetime Pointer to datetime_data_t to be configured.
 * @param key Keypad input for navigation and digit entry.
 * @retval true if adjustment is complete, false otherwise.
 */
static bool_t display_config_datetime(datetime_data_t *datetime, uint8_t key)
{
  if(datetime == NULL || key <= '#' || key >= 'D') Error_Handler();

    uint8_t buffer[BUFFER_S] = {0};
    // Screen positions for digit entry
    static uint8_t cursor_positions[] = {6, 7, 9, 10, 12, 13};
    // Temporary array for time: HHMMSS
    static uint8_t time_digits[TIME_DIGITS_LENGTH] = {0};
    // Temporary array for date: DDMMYY
    static uint8_t date_digits[DATE_DIGITS_LENGTH] = {0};
    static uint8_t current_cursor_position_x = 0;
    static uint8_t current_cursor_position_y = 0;
    static bool_t adjustment_started = false;
    bool_t ret = false;

    if (!adjustment_started) {
        // Initialize the configuration screen
        LCD_show_cursor_blink();

        LCD_set_position_xy(0, 0);
        sprintf((char*)buffer, "Conf. %02d:%02d:%02d", datetime->time.hours, datetime->time.minutes, datetime->time.seconds);
        LCD_print_string(buffer);

        LCD_set_position_xy(0, 1);
        sprintf((char*)buffer, "      %02d/%02d/%02d", datetime->date.date, datetime->date.month, datetime->date.year);
        LCD_print_string(buffer);

        // Initialize temporary arrays
        time_digits[0] = datetime->time.hours / 10;
        time_digits[1] = datetime->time.hours % 10;
        time_digits[2] = datetime->time.minutes / 10;
        time_digits[3] = datetime->time.minutes % 10;
        time_digits[4] = datetime->time.seconds / 10;
        time_digits[5] = datetime->time.seconds % 10;

        date_digits[0] = datetime->date.date / 10;
        date_digits[1] = datetime->date.date % 10;
        date_digits[2] = datetime->date.month / 10;
        date_digits[3] = datetime->date.month % 10;
        date_digits[4] = datetime->date.year / 10;
        date_digits[5] = datetime->date.year % 10;

        adjustment_started = true;
    }

    // Set cursor position
    LCD_set_position_xy(cursor_positions[current_cursor_position_x], current_cursor_position_y);

    // Navigation and digit entry
    switch (key) {
        case '*':
            move_cursor_backward(&current_cursor_position_x, &current_cursor_position_y, sizeof(cursor_positions));
            break;
        case '#':
            move_cursor_forward(&current_cursor_position_x, &current_cursor_position_y, sizeof(cursor_positions));
            break;
        case 'A':
            adjustment_started = false;
            current_cursor_position_x = 0;
            current_cursor_position_y = 0;
            ret = true;
            break;
    }

    if (key >= '0' && key <= '9') {
        uint8_t digit = key - '0';
        LCD_print_char(key);

        // Fill temporary arrays
        if (current_cursor_position_y == 0) {
            time_digits[current_cursor_position_x] = digit;
        } else {
            date_digits[current_cursor_position_x] = digit;
        }

        // Mover el cursor a la siguiente posición
        move_cursor_forward(&current_cursor_position_x, &current_cursor_position_y, sizeof(cursor_positions));
    }

    // Update datetime structure if adjustment is complete
    if (ret) {
        datetime->time.hours = (time_digits[0] * 10) + time_digits[1];
        datetime->time.minutes = (time_digits[2] * 10) + time_digits[3];
        datetime->time.seconds = (time_digits[4] * 10) + time_digits[5];

        datetime->date.date = (date_digits[0] * 10) + date_digits[1];
        datetime->date.month = (date_digits[2] * 10) + date_digits[3];
        datetime->date.year = (date_digits[4] * 10) + date_digits[5];

        // Adjust to valid values
        RTC_validate_datetime(datetime);
        RTC_calculate_day_of_week(datetime);
    }

    return ret;
}

/**
 * @brief Manages time and date adjustment through the keypad.
 * 
 * Handles timeout and determines when the adjustment is ready to be saved.
 * 
 * @param data_time_struct Pointer to datetime_data_t to be adjusted.
 * @retval time_adjust_t Status indicating adjustment progress or timeout.
 */
static time_adjust_t get_time_from_keypad(datetime_data_t *data_time_struct)
{
  if(data_time_struct == NULL) Error_Handler();

  static delay_t timeout;
  static bool_t delay_was_initialized = false;
  time_adjust_t ret = ADJUSTING;
  uint8_t key = keypad_get_key();

  if(!delay_was_initialized || key != '\0')
  {
    DelayInit(&timeout,ADJUST_TIMEOUT);
    delay_was_initialized = true;
  }
  if(DelayRead(&timeout))
    ret = TIMEOUT_REACHED;
  
  if(display_config_datetime(data_time_struct, key)){
    ret = READY_FOR_SAVE;
    delay_was_initialized = false;
  }

  if(ret != ADJUSTING)
    LCD_clear();

  return ret;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
