# NUCLEO-F411RE 
## FEATURES
- 16 PWM for control 16 servo SG90 
- Service UART for config number of servo and PWM (max, min).

## PACKET UART FORMAT FOR CONFIG SERVO AND PWM
- This section shows the serialized message format of packets.
 - Below is the over-the-wire format for packet (the in-memory representation might differ).

![PACKET](/img/packet.png)

| Byte index    |      C version                |  Value    | Explanation                                   |
|:----------:   | -------------                 | ------    |------                                         |
| 0             | ```uint8_t beginByte```       | 0x9B      | Begin Bytes                                   |
| 1             | ```uint8_t dataLength```      | 0x06      | Frame Length                                  |
| 2             | ```uint8_t channel```         | 0 - 16    | Number of channel servo that want to control  |
| 3             | ```uint8_t min_pwm```         | 10 - 25   | Min PWM __(1 -> 2.5)ms *10__                  |
| 4             | ```uint8_t max_pwm```         | 10 - 25   | Max PWM __(1 -> 2.5)ms *10__                  |
| 5             | ```uint8_t endByte```         | 0xFE      | End Bytes                                     |