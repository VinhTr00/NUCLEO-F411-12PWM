/* Includes ------------------------------------------------------------------*/
#include "handle_interrupt.h"
#include "cmsis_os.h"
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HANDLE_INTERRUPT_THREAD_PRIO      (osPriority_t) osPriorityNormal
#define COUNTING_INTERRUPT_THREAD_PRIO    (osPriority_t) osPriorityNormal

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadId_t AUXHandle, CountingHandle;
osSemaphoreId_t xSemaphore1, xSemaphore2, xSemaphore3, xSemaphore4,
				xSemaphore5, xSemaphore6, xSemaphore7, xSemaphore8;

osMessageQueueId_t queue_flag;
osMessageQueueId_t queue_count;

/* Private function prototypes -----------------------------------------------*/
static void startHandleInterrupt(void * argument);
static void counting(void * argument);

/* Private functions ---------------------------------------------------------*/
static void semaphore_init(void){
	xSemaphore1 = osSemaphoreNew(1, 0, NULL);
	xSemaphore2 = osSemaphoreNew(1, 0, NULL);
	xSemaphore3 = osSemaphoreNew(1, 0, NULL);
	xSemaphore4 = osSemaphoreNew(1, 0, NULL);
	xSemaphore5 = osSemaphoreNew(1, 0, NULL);
	xSemaphore6 = osSemaphoreNew(1, 0, NULL);
	xSemaphore7 = osSemaphoreNew(1, 0, NULL);
	xSemaphore8 = osSemaphoreNew(1, 0, NULL);
}

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
	semaphore_init();
	queue_flag = osMessageQueueNew(1, sizeof(bool), NULL);
	queue_count = osMessageQueueNew(1, sizeof(uint8_t), NULL);
}

static void counting(void * argument){
	uint8_t count = 0;
	bool flag = false;
	while (1){
		if (HAL_GPIO_ReadPin(BT_GPIO_Port, BT_Pin) == 0){
			osDelay(100);
			if (HAL_GPIO_ReadPin(BT_GPIO_Port, BT_Pin) == 0){
				count++;
				if (count > 8) count = 0;
				flag = true;
			}
			else flag = false;
			osMessageQueuePut(queue_flag,  &flag,  0, 0);
			osMessageQueuePut(queue_count, &count, 0, 0);
		}
	}
}

static void startHandleInterrupt(void * argument){
	uint8_t xCount;
	bool xFlag;
	osStatus_t flagStatus, countStatus;
	while (1){
		flagStatus = osMessageQueueGet(queue_flag, &xFlag, 0, 0);
		countStatus = osMessageQueueGet(queue_count, &xCount, 0, 0);
		if ((flagStatus == osOK) && (flagStatus == osOK))
		{
			if (xFlag) {
				switch (xCount){
					case 1:
						osSemaphoreRelease(xSemaphore1);
						break;
					case 2:
						osSemaphoreRelease(xSemaphore2);
						break;
					case 3:
						osSemaphoreRelease(xSemaphore3);
						break;
					case 4:
						osSemaphoreRelease(xSemaphore4);
						break;
					case 5:
						osSemaphoreRelease(xSemaphore5);
						break;
					case 6:
						osSemaphoreRelease(xSemaphore6);
						break;
					case 7:
						osSemaphoreRelease(xSemaphore7);
						break;
					case 8:
						osSemaphoreRelease(xSemaphore8);
						break;
				}
			}
		}
	}
}
