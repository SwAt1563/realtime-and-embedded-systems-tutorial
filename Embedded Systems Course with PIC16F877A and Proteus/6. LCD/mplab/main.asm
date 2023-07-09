
PROCESSOR 16F877A
INCLUDE "P16F877A.INC"	


__CONFIG 0x3731



Char EQU 0x25





; The instructions should start from here
ORG 0x00
GOTO init


ORG 0x04
GOTO ISR


; The init for our program
init:



BANKSEL TRISB
BSF TRISB, TRISB0

BANKSEL INTCON
BSF INTCON, GIE
BSF INTCON, INTE

BANKSEL TRISD
CLRF TRISD

BANKSEL PORTD


MOVLW 'A'
MOVWF Char









GOTO start


; When intruput happend the program will enter here
ISR:
BANKSEL INTCON
BCF INTCON, INTE
BCF INTCON, INTF

; TASK


INCF Char



; END TASK

BANKSEL INTCON
BSF INTCON, INTE

BANKSEL PORTD
retfie





;INCLUDE "LCDIS_PORTD.INC" ; IF U WANT TO USE LCD ON PORT D
INCLUDE "LCDIS_PORTA.INC" ; IF U WANT TO USE LCD ON PORT A

; The main code for our program
start:

CALL xms
CALL xms


CALL inid


loop:

MOVFW Char
BSF Select, RS
CALL send

GOTO loop





END

