
PROCESSOR 16F877A
INCLUDE "P16F877A.INC"	


__CONFIG 0x3731

; The instructions should start from here
ORG 0x00
GOTO init


ORG 0x04
GOTO ISR



; When intruput happend the program will enter here
ISR:

NOP

retfie





; The init for our program
init:

BANKSEL TRISD
CLRF TRISD




NOP

GOTO start

; The main code for our program
start:



call LED_ON

loop:


GOTO loop



; LED ON PORTD2
LED_ON:

BANKSEL PORTD
BSF PORTD, RD2

return


END

