.ORIG x3000 ;         start the program at location x3000

ADD R6,R6,x1

ADD R5,R5,x1;         R5为指令计数器

LOOP2 ADD R5,R5,x2;
AND R2,R1,R6
BRz LOOP1
ADD R7,R7,R0
ADD R5,R5,x1
LOOP1 ADD R0,R0,R0
ADD R5,R5,x3
ADD R6,R6,R6
BRnp LOOP2

HALT ;                halt
.END