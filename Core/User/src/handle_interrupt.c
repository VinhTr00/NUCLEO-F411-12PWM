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
osThreadId_t AUXHandle;

osSemaphoreId_t xSemaphore;

osMessageQueueId_t queue_count;

static volatile bool flagUP = false, flagDOWN = false;

/* Private function prototypes -----------------------------------------------*/
static void startHandleInterrupt(void * argument);

/* Private functions ---------------------------------------------------------*/
static void init_queue_semaphore(void){
	xSemaphore = osSemaphoreNew(1, 0, NULL);
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
	init_queue_semaphore();
}

static void startHandleInterrupt(void * argument){
	while (1){
		if (HAL_GPIO_ReadPin(BT_GPIO_Port, BT_Pin) == 0){
			flagUP = true;
			if (flagUP && flagDOWN) {
				osSemaphoreRelease(xSemaphore);
				flagDOWN = false;
			}
		}
		else {
			flagDOWN = true;
			flagUP = false;
		}
	}
}