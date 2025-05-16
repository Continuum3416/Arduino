DATA SEGMENT USE16
MESG DB 'Hello World', '$'
DATA ENDS
CODE SEGMENT USE16
    ASSUME CS:CODE, DS:DATA
BEG:
    mov ax, DATA
    mov ds, ax
    mov ah, 9
    mov dx, OFFSET MESG
    int 21h
    mov ah, 4ch
    int 21h
CODE ENDS
END BEG