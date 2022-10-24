/* Includes ------------------------------------------------------------------*/
#include "handle_interrupt.h"
#include "cmsis_os.h"
#include "service_uart.h"
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HANDLE_INTERRUPT_THREAD_PRIO      (osPriority_t) osPriorityNormal
#define COUNTING_INTERRUPT_THREAD_PRIO    (osPriority_t) osPriorityNormal

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadId_t AUXHandle, CountingHandle;
osSemaphoreId_t xSemaphore1, xSemaphore2,  xSemaphore3,  xSemaphore4,
				xSemaphore5, xSemaphore6,  xSemaphore7,  xSemaphore8,
				xSemaphore9, xSemaphore10, xSemaphore11, xSemaphore12;

osMessageQueueId_t queue_count;
extern osMessageQueueId_t queue_config_servo;
static volatile bool flagUP = false, flagDOWN = false;
static uint8_t count = 0;
ServiceUart numberServo;
/* Private function prototypes -----------------------------------------------*/
static void startHandleInterrupt(void * argument);
static void counting(void * argument);

/* Private functions ---------------------------------------------------------*/
static void init_queue_semaphore(void){
	numberServo.channel = 9;
	xSemaphore1  = osSemaphoreNew(1, 0, NULL);
	xSemaphore2  = osSemaphoreNew(1, 0, NULL);
	xSemaphore3  = osSemaphoreNew(1, 0, NULL);
	xSemaphore4  = osSemaphoreNew(1, 0, NULL);
	xSemaphore5  = osSemaphoreNew(1, 0, NULL);
	xSemaphore6  = osSemaphoreNew(1, 0, NULL);
	xSemaphore7  = osSemaphoreNew(1, 0, NULL);
	xSemaphore8  = osSemaphoreNew(1, 0, NULL);
	xSemaphore9  = osSemaphoreNew(1, 0, NULL);
	xSemaphore10 = osSemaphoreNew(1, 0, NULL);
	xSemaphore11 = osSemaphoreNew(1, 0, NULL);
	xSemaphore12 = osSemaphoreNew(1, 0, NULL);
	queue_count  = osMessageQueueNew(1, sizeof(uint8_t), NULL);
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
	while (1){
		osMessageQueueGet(queue_config_servo, &numberServo, 0, 0);
		if (HAL_GPIO_ReadPin(BT_GPIO_Port, BT_Pin) == 0){
			flagUP = true;
			if (flagUP && flagDOWN) {
				count++;
				if (count > numberServo.channel) count = 0;
				osMessageQueuePut(queue_count, &count, 0, 0);
				flagDOWN = false;
			}
		}
		else {
			flagDOWN = true;
			flagUP = false;
		}
	}
}

static void startHandleInterrupt(void * argument){
	osStatus_t countStatus;
	static uint8_t xCount;
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
				case 9:
					osSemaphoreRelease(xSemaphore9);
					break;
				case 10:
					osSemaphoreRelease(xSemaphore10);
					break;
				case 11:
					osSemaphoreRelease(xSemaphore11);
					break;
				case 12:
					osSemaphoreRelease(xSemaphore12);
					break;
			}
		}
	}
}
