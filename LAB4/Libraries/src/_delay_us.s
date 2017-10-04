			AREA	delay, CODE
			EXPORT	_delay_us	;make _delay_us visible
			ALIGN
_delay_us	PROC
			
			PUSH {r4, lr}
outer		MOV  r1, #4
inner		SUBS r1, r1, #1 
			BNE  inner
			SUBS r0, r0, #1
			BNE  outer
			POP	 {r4, pc}
			ENDP
			END

