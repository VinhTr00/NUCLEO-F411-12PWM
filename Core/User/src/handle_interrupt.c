/* Includes ------------------------------------------------------------------*/
#include "handle_interrupt.h"
#include "cmsis_os.h"
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HANDLE_INTERRUPT_THREAD_PRIO    (osPriority_t) osPriorityNormal
#define COUNTING_INTERRUPT_THREAD_PRIO    (osPriority_t) osPriorityNormal

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadId_t AUXHandle, CountingHandle;
extern osSemaphoreId_t xSemaphore1, xSemaphore2, xSemaphore3, xSemaphore4,
					   xSemaphore5, xSemaphore6, xSemaphore7, xSemaphore8;
volatile static int count = 0;
static bool flag = false;

/* Private function prototypes -----------------------------------------------*/
static void startHandleInterrupt(void * argument);
static void counting(void * argument);

/* Private functions ---------------------------------------------------------*/
void handle_AUX_signal_init(void){
	const osThreadAttr_t AUX_Task = {
			.name = "AUX_signal_task",
			.stack_size = 256*4,
			.priority = HANDLE_INTERRUPT_THREAD_PRIO,
	};
	AUXHandle = osThreadNew(startHandleInterrupt, NULL, &AUX_Task);
	(void)AUXHandle;

	const osThreadAttr_t countingTask = {
			.name = "counting_signal_task",
			.stack_size = 256*4,
			.priority = COUNTING_INTERRUPT_THREAD_PRIO,
	};
	CountingHandle = osThreadNew(counting, NULL, &countingTask);
	(void)CountingHandle;
}

static void counting(void * argument){
	while (1){
		if (HAL_GPIO_ReadPin(BT_GPIO_Port, BT_Pin) == 0){
			osDelay(100);
			if (HAL_GPIO_ReadPin(BT_GPIO_Port, BT_Pin) == 0){
				count++;
				if (count > 8) count = 0;
				flag = true;
			}
			else flag = false;
		}
	}
}

static void startHandleInterrupt(void * argument){
	while (1){
		if (flag) {
			switch (count){
				case 1:
					osSemaphoreRelease(xSemaphore1);
					flag = false;
					break;
				case 2:
					osSemaphoreRelease(xSemaphore2);
					flag = false;
					break;
				case 3:
					osSemaphoreRelease(xSemaphore3);
					flag = false;
					break;
				case 4:
					osSemaphoreRelease(xSemaphore4);
					flag = false;
					break;
				case 5:
					osSemaphoreRelease(xSemaphore5);
					flag = false;
					break;
				case 6:
					osSemaphoreRelease(xSemaphore6);
					flag = false;
					break;
				case 7:
					osSemaphoreRelease(xSemaphore7);
					flag = false;
					break;
				case 8:
					osSemaphoreRelease(xSemaphore8);
					flag = false;
					break;
			}
		}
	}
}
