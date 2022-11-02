/* Includes ------------------------------------------------------------------*/
#include "servo.h"
#include "cmsis_os.h"
#include "tim.h"
#include "service_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SERVO_THREAD_PRIO    (osPriority_t) osPriorityNormal
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadId_t servoHandle, dataHandle;
osMessageQueueId_t queue_data, queue_servo;

extern osSemaphoreId_t xSemaphore;

extern osMessageQueueId_t queue_config;

/* Private function prototypes -----------------------------------------------*/
static void StartServo(void * argument);
static void StartHandleData(void * argument);
/* Private functions ---------------------------------------------------------*/

static void timer_pwm_init(void){
	//TIM 1: Channel 1->4
	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);

	//TIM 3: Channel 1->4
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);

	//TIM 4: Channel 1->4
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);

	queue_data = osMessageQueueNew(10, sizeof(uint8_t), NULL);
	queue_servo = osMessageQueueNew(1, sizeof(ServiceUart), NULL);
}

void servo_init(void){
	const osThreadAttr_t servoTask = {
		.name = "ServoTask",
		.stack_size = 256*4,
		.priority = SERVO_THREAD_PRIO,
	};
	servoHandle = osThreadNew(StartServo, NULL, &servoTask);
	(void)servoHandle;

	const osThreadAttr_t dataTask = {
		.name = "DataTask",
		.stack_size = 256*4,
		.priority = SERVO_THREAD_PRIO,
	};
	dataHandle = osThreadNew(StartHandleData, NULL, &dataTask);
	(void)dataHandle;

	timer_pwm_init();
}

void Start_PWM_Servo(TIM_HandleTypeDef *pTim, uint16_t channel, uint16_t pwm_max, uint16_t pwm_min){
	__HAL_TIM_SetCompare(pTim, channel, pwm_max);
	osDelay(2000);
	__HAL_TIM_SetCompare(pTim, channel, pwm_min);
	osDelay(500);
	__HAL_TIM_SetCompare(pTim, channel, 0);
}

static void StartHandleData(void * argument){
	ServiceUart config;
	uint8_t counting = 0, number_servo = 9;
	while (1){
		if (osMessageQueueGet(queue_config, &config, 0, 0) == osOK){
			number_servo = config.channel;
			osMessageQueuePut(queue_servo, &config, 0, 0);
		}
		if (osSemaphoreAcquire(xSemaphore, pdMS_TO_TICKS(50)) == osOK){
			counting++;
			if (counting > number_servo) counting = 0;
			osMessageQueuePut(queue_data, &counting, 0, 0);
		}
	}
}

static void StartServo(void * argument){
	ServiceUart config;
	uint8_t mode = 0;
	uint16_t max_pwm = 2000, min_pwm = 1000;
	while (1){
		osMessageQueueGet(queue_data, &mode, 0 , 0);
		if (osMessageQueueGet(queue_servo, &config, 0, 0) == osOK){
			max_pwm = (config.max_pwm)*100;
			min_pwm = (config.min_pwm)*100;
		}
		switch (mode)
		{
			case 0:
				break;
			case 1:
				Start_PWM_Servo(&htim3, TIM_CHANNEL_1, max_pwm, min_pwm);
				break;
			case 2:
				Start_PWM_Servo(&htim3, TIM_CHANNEL_2, max_pwm, min_pwm);
				break;
			case 3:
				Start_PWM_Servo(&htim3, TIM_CHANNEL_3, max_pwm, min_pwm);
				break;
			case 4:
				Start_PWM_Servo(&htim3, TIM_CHANNEL_4, max_pwm, min_pwm);
				break;
			case 5:
				Start_PWM_Servo(&htim4, TIM_CHANNEL_1, max_pwm, min_pwm);
				break;
			case 6:
				Start_PWM_Servo(&htim4, TIM_CHANNEL_2, max_pwm, min_pwm);
				break;
			case 7:
				Start_PWM_Servo(&htim4, TIM_CHANNEL_3, max_pwm, min_pwm);
				break;
			case 8:
				Start_PWM_Servo(&htim4, TIM_CHANNEL_4, max_pwm, min_pwm);
				break;
			case 9:
				Start_PWM_Servo(&htim1, TIM_CHANNEL_1, max_pwm, min_pwm);
				break;
			case 10:
				Start_PWM_Servo(&htim1, TIM_CHANNEL_2, max_pwm, min_pwm);
				break;
			case 11:
				Start_PWM_Servo(&htim1, TIM_CHANNEL_3, max_pwm, min_pwm);
				break;
			case 12:
				Start_PWM_Servo(&htim1, TIM_CHANNEL_4, max_pwm, min_pwm);
				break;
			default:
				break;
		}
	}
}
