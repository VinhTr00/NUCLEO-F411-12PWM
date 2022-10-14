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

osMessageQueueId_t queue_count;

/* Private function prototypes -----------------------------------------------*/
static void startHandleInterrupt(void * argument);
static void counting(void * argument);

/* Private functions ---------------------------------------------------------*/
static void init_queue_semaphore(void){
	xSemaphore1 = osSemaphoreNew(1, 0, NULL);
	xSemaphore2 = osSemaphoreNew(1, 0, NULL);
	xSemaphore3 = osSemaphoreNew(1, 0, NULL);
	xSemaphore4 = osSemaphoreNew(1, 0, NULL);
	xSemaphore5 = osSemaphoreNew(1, 0, NULL);
	xSemaphore6 = osSemaphoreNew(1, 0, NULL);
	xSemaphore7 = osSemaphoreNew(1, 0, NULL);
	xSemaphore8 = osSemaphoreNew(1, 0, NULL);
	queue_count = osMessageQueueNew(1, sizeof(uint8_t), NULL);
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
	init_queue_semaphore();

}

static void counting(void * argument){
	static uint8_t count = 0;
	while (1){
		if (HAL_GPIO_ReadPin(BT_GPIO_Port, BT_Pin) == 0){
			osDelay(100);
			if (HAL_GPIO_ReadPin(BT_GPIO_Port, BT_Pin) == 0){
				count++;
				if (count > 8) count = 0;
				osMessageQueuePut(queue_count, &count, 0, 0);
			}
		}
	}
}

static void startHandleInterrupt(void * argument){
	osStatus_t countStatus;
	uint8_t xCount;
	while (1){
		countStatus = osMessageQueueGet(queue_count, &xCount, 0, 0);
		if (countStatus == osOK)
		{
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
