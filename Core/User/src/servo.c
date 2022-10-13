/* Includes ------------------------------------------------------------------*/
#include "servo.h"
#include "cmsis_os.h"
#include "tim.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SERVO_THREAD_PRIO    (osPriority_t) osPriorityNormal
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadId_t servoHandle;
extern osSemaphoreId_t xSemaphore1, xSemaphore2, xSemaphore3, xSemaphore4,
					   xSemaphore5, xSemaphore6, xSemaphore7, xSemaphore8;

/* Private function prototypes -----------------------------------------------*/
static void StartServo(void * argument);
/* Private functions ---------------------------------------------------------*/
void timer_pwm_init(void){
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

static void StartServo(void * argument){
	while (1){
		if (osSemaphoreAcquire(xSemaphore1, pdMS_TO_TICKS(50)) == osOK){
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,1000);
			osDelay(2000);
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_1,0);
		}
		else if (osSemaphoreAcquire(xSemaphore2, pdMS_TO_TICKS(50)) == osOK){
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,1000);
			osDelay(2000);
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_2,0);
		}
		else if (osSemaphoreAcquire(xSemaphore3, pdMS_TO_TICKS(50)) == osOK){
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,1000);
			osDelay(2000);
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_3,0);
		}
		else if (osSemaphoreAcquire(xSemaphore4, pdMS_TO_TICKS(50)) == osOK){
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,1000);
			osDelay(2000);
			__HAL_TIM_SetCompare(&htim3,TIM_CHANNEL_4,0);
		}
		else if (osSemaphoreAcquire(xSemaphore5, pdMS_TO_TICKS(50)) == osOK){
			__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,1000);
			osDelay(2000);
			__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,0);
		}
		else if (osSemaphoreAcquire(xSemaphore6, pdMS_TO_TICKS(50)) == osOK){
			__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,1000);
			osDelay(2000);
			__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,0);
		}
		else if (osSemaphoreAcquire(xSemaphore7, pdMS_TO_TICKS(50)) == osOK){
			__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,1000);
			osDelay(2000);
			__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,0);
		}
		else if (osSemaphoreAcquire(xSemaphore8, pdMS_TO_TICKS(50)) == osOK){
			__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,1000);
			osDelay(2000);
			__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4,0);
		}
	}
}
