.ORIG x3000         ;2*R7=R1←R7←R2←R3
ADD R7,R7,#1
ADD R2,R2,#1
ADD R3,R3,#2
LD R6,forAND
LOOP ADD R1,R7,R7
ADD R7,R2,#0
AND R7,R7,R6
ADD R2,R3,#0
ADD R3,R1,R2
ADD R0,R0,#-1
BRp LOOP
forAND .FILL x03FF
Fa 
.FILL x03A2
Fb .FILL x0002
Fc .FILL x000A
Fd .FILL x0356
.END