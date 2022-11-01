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
osThreadId_t servoHandle;
extern osSemaphoreId_t xSemaphore1, xSemaphore2, xSemaphore3, xSemaphore4,
					   xSemaphore5, xSemaphore6, xSemaphore7, xSemaphore8,
					   xSemaphore9, xSemaphore10, xSemaphore11, xSemaphore12;
extern osMessageQueueId_t queue_config_pwm;
ServiceUart configPWM;
uint16_t max_pwm, min_pwm;
/* Private function prototypes -----------------------------------------------*/
static void StartServo(void * argument);
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

	configPWM.min_pwm = 10;
	configPWM.max_pwm = 20;
	min_pwm = (configPWM.min_pwm)*100;
	max_pwm = (configPWM.max_pwm)*100;
}

void servo_init(void){
	const osThreadAttr_t servoTask = {
		.name = "ServoTask",
		.stack_size = 256*4,
		.priority = SERVO_THREAD_PRIO,
	};
	servoHandle = osThreadNew(StartServo, NULL, &servoTask);
	(void)servoHandle;
	timer_pwm_init();
}

void Start_PWM_Servo(TIM_HandleTypeDef *pTim, uint16_t channel, uint16_t pwm_max, uint16_t pwm_min){
	__HAL_TIM_SetCompare(pTim, channel, pwm_max);
	osDelay(2000);
	__HAL_TIM_SetCompare(pTim, channel, pwm_min);
}

static void StartServo(void * argument){
	while (1){
		if (osMessageQueueGet(queue_config_pwm, &configPWM, 0, 0) == osOK){
			min_pwm = (configPWM.min_pwm)*100;
			max_pwm = (configPWM.max_pwm)*100;
		}
		if (osSemaphoreAcquire(xSemaphore1, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim3, TIM_CHANNEL_1, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore2, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim3, TIM_CHANNEL_2, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore3, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim3, TIM_CHANNEL_3, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore4, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim3, TIM_CHANNEL_4, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore5, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim4, TIM_CHANNEL_1, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore6, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim4, TIM_CHANNEL_2, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore7, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim4, TIM_CHANNEL_3, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore8, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim4, TIM_CHANNEL_4, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore9, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim1, TIM_CHANNEL_1, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore10, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim1, TIM_CHANNEL_2, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore11, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim1, TIM_CHANNEL_3, max_pwm, min_pwm);
		}
		else if (osSemaphoreAcquire(xSemaphore12, pdMS_TO_TICKS(50)) == osOK){
			Start_PWM_Servo(&htim1, TIM_CHANNEL_4, max_pwm, min_pwm);
		}
	}
}
