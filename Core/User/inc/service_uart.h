
#ifndef _SERVICE_UART_H_
#define _SERVICE_UART_H_

#include "main.h"

typedef enum {
	PARSE_IDLE = 0,
	BEGIN_BYTE,
	DATA_LENGTH,
	CHANNEL,
	MIN_PWM,
	MAX_PWM,
	END_BYTE
} ReceiveStatusType;

typedef enum {
	PARSE_ERROR = -1,
	PARSE_OK,
	PARSE_WIP,
	PARSE_COMPLETE
} ParseStatusType;

typedef struct {
	uint8_t beginByte;
	uint8_t dataLength;
	uint8_t channel;
	uint8_t min_pwm;
	uint8_t max_pwm;
	uint8_t endByte;
	ParseStatusType parseStatus;
} ServiceUart;

void init_service_uart(void);

#endif /* _SERVICE_UART_H_ */
