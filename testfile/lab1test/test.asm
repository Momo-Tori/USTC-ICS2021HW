.ORIG x3000 ; start the program at location x3000

LOOP ADD R7, R0, R7
ADD R1, R1, #-1
BRnp LOOP

HALT ; halt
.END
