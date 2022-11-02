#include "service_uart.h"
#include "ringbuf.h"
#include "cmsis_os.h"
#include "usart.h"

#define RX_BUFFER_SIZE 1
#define RINGBUF_BUFF_LEN 50
#define UART_THREAD_PRIO    (osPriority_t) osPriorityNormal

static ReceiveStatusType ReceiverUART6;
static RINGBUF ring;
static volatile RINGBUF *ringbuf;

uint8_t aRxBuffer, value, ptBufferUART6[RINGBUF_BUFF_LEN];
ServiceUart serviceUART;
osMessageQueueId_t queue_config;
uint8_t TxBuffOK[10] = "OK!!!\r\n";

void init_service_uart(void){
	ringbuf = &ring;
	RINGBUF_Init((RINGBUF*)ringbuf, ptBufferUART6, RINGBUF_BUFF_LEN);
	queue_config = osMessageQueueNew(1, sizeof(ServiceUart), NULL);
	HAL_UART_Receive_IT(&huart6, &aRxBuffer, RX_BUFFER_SIZE);
}

ParseStatusType service_uart_parse(uint8_t recByte, ServiceUart *serviceUart, ReceiveStatusType *recStatus){
	if (recByte == 0x9B){
		*recStatus = BEGIN_BYTE;
	}
	switch (*recStatus){
		case PARSE_IDLE:
			serviceUart->parseStatus = PARSE_ERROR;
			return PARSE_ERROR;
			break;
		case BEGIN_BYTE:
			if (recByte != 0x9B) *recStatus = PARSE_IDLE;
			else {
				serviceUart->beginByte = recByte;
				serviceUart->parseStatus = PARSE_WIP;
				*recStatus = DATA_LENGTH;
			}
			break;
		case DATA_LENGTH:
			if (recByte != 0x06) *recStatus = PARSE_IDLE;
			else {
				serviceUart->dataLength = recByte;
				serviceUart->parseStatus = PARSE_WIP;
				*recStatus = CHANNEL;
			}
			break;
		case CHANNEL:
			if (recByte > 16) *recStatus = PARSE_IDLE;
			else {
				serviceUart->channel = recByte;
				serviceUart->parseStatus = PARSE_WIP;
				*recStatus = MIN_PWM;
			}
			break;
		case MIN_PWM:
			if (recByte<10 || recByte>25) *recStatus = PARSE_IDLE;
			else {
				serviceUart->min_pwm = recByte;
				serviceUart->parseStatus = PARSE_WIP;
				*recStatus = MAX_PWM;
			}
			break;
		case MAX_PWM:
			if (recByte<10 || recByte>25) *recStatus = PARSE_IDLE;
			else {
				serviceUart->max_pwm = recByte;
				serviceUart->parseStatus = PARSE_WIP;
				*recStatus = END_BYTE;
			}
			break;
		case END_BYTE:
			if (recByte != 0xFE) *recStatus = PARSE_IDLE;
			else {
				serviceUart->endByte = recByte;
				serviceUart->parseStatus = PARSE_COMPLETE;
				*recStatus = BEGIN_BYTE;
				return PARSE_OK;
			}
			break;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (HAL_UART_Receive_IT(&huart6, &aRxBuffer, RX_BUFFER_SIZE) == HAL_OK){
		RINGBUF_Put((RINGBUF *) ringbuf, aRxBuffer);
		RINGBUF_Get((RINGBUF *) ringbuf, &value);
		if (service_uart_parse(value, &serviceUART, &ReceiverUART6) == PARSE_OK){
			osMessageQueuePut(queue_config, &serviceUART, 0, 0);
			HAL_UART_Transmit(&huart6, TxBuffOK, sizeof(TxBuffOK), 1000);
		}
	}
}
