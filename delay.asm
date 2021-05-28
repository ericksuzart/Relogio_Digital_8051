; delay

public	delay
          
prog	segment	code 
	rseg	prog

delay:
		mov 		r6, #6			; Delay  2,08ms para um clock de 12Mhz
L1:
	mov 		r5, #172
	djnz		r5, $
	djnz		r6, L1
	ret
	end