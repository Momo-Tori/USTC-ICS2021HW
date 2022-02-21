.ORIG x3000
LEA R6,STACK
JSR JUDGE
HALT

JUDGE STR R2,R6,#0
ADD R6,R6,#1
STR R3,R6,#0
ADD R6,R6,#1
STR R4,R6,#0
ADD R6,R6,#1
STR R5,R6,#0
ADD R6,R6,#1
STR R7,R6,#0
ADD R6,R6,#1

AND R3,R3,#0
ADD R3,R3,#2;R3=i=2
LOOP0 ADD R1,R3,#0
ADD R2,R3,#0
JSR Times
NOT R1,R1
ADD R1,R1,#1
ADD R1,R0,R1
BRn True
ADD R1,R3,#0
JSR Mod
ADD R1,R1,#0
BRz False
ADD R3,R3,#1
BR LOOP0
True AND R1,R1,#0
ADD R1,R1,#1
BR Finish
False AND R1,R1,#0
Finish ADD R6,R6,#-1
LDR R7,R6,#0
ADD R6,R6,#-1
LDR R5,R6,#0
ADD R6,R6,#-1
LDR R4,R6,#0
ADD R6,R6,#-1
LDR R3,R6,#0
ADD R6,R6,#-1
LDR R2,R6,#0
RET

;input:R1,R2,output:R1=R1*R2
;R4，R5为调用者保存
Times STR R3,R6,#0
ADD R6,R6,#1

ADD R3,R1,#0
AND R1,R1,#0
AND R4,R4,#0
ADD R4,R4,x1
LOOP2 AND R5,R2,R4
BRz LOOP1
ADD R1,R1,R3
LOOP1 ADD R3,R3,R3
ADD R4,R4,R4
BRnp LOOP2

ADD R6,R6,#-1
LDR R3,R6,#0
RET

;input:R1,R0,output:R1=R0%R1
;R2，R5为调用者保存，R0不变const
Mod 
;试商法计算
NOT R2,R1
ADD R2,R2,#1
AND R1,R1,#0
STR R1,R6,#0;设置结束条件
ADD R6,R6,#1
ADD R1,R0,#0
LOOP3 ADD R5,R2,R0
BRn LOOP4
STR R2,R6,#0
ADD R6,R6,#1
ADD R2,R2,R2
BR LOOP3
LOOP4 ADD R6,R6,#-1
LDR R2,R6,#0
BRz Fin
ADD R5,R2,R1
BRn skip
ADD R1,R2,R1
skip BR LOOP4
Fin RET

STACK .BLKW #40
.END