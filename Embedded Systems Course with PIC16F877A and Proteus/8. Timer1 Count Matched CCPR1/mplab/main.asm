
PROCESSOR 16F877A
INCLUDE "P16F877A.INC"	


__CONFIG 0x3731







; The instructions should start from here
ORG 0x00
GOTO init


ORG 0x04
GOTO ISR


; The init for our program
init:

BANKSEL TRISD
BCF TRISD, RD2

BANKSEL CCP1CON
MOVLW b'00001010'
MOVWF CCP1CON

BANKSEL T1CON
MOVLW b'00000001'
MOVWF T1CON


BANKSEL CCPR1H 
;MOVLW b'00000010'
;MOVFW CCPR1H 
BSF CCPR1H, 1


BANKSEL CCPR1L 
;MOVLW b'00110000'
;MOVFW CCPR1L 
BSF CCPR1L, 4
BSF CCPR1L, 5

BANKSEL INTCON
BSF INTCON, GIE
BSF INTCON, PEIE

BANKSEL PIE1
BSF PIE1, 2








BANKSEL PORTD
GOTO start


; When intruput happend the program will enter here
ISR:
BANKSEL PIR1
BCF PIR1, 2
BANKSEL PIE1
BCF PIE1, 2



BANKSEL PORTD

BTFSS PORTD, RD2
GOTO LED_TURN_ON ; WHEN RD2: 0

GOTO LED_TURN_OFF; WHEN RD2: 1

BACK:







BANKSEL PIE1
BSF PIE1, 2

BANKSEL PORTD
retfie






; The main code for our program
start:




loop:



GOTO loop



LED_TURN_ON:

BANKSEL PORTD
BSF PORTD, RD2 

GOTO BACK


LED_TURN_OFF:

BANKSEL PORTD
BCF PORTD, RD2 

GOTO BACK



END
