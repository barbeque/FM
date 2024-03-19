				.386p

				PUBLIC	POLL_TIMER

				ASSUME CS:CODE

CODE			SEGMENT

POLL_TIMER		PROC
				PUSH	ECX

				MOV		ECX,100

POLL_OUTER_LOOP:
				IN		AL,060H
				SHR		AL,2
				AND		AL,7
				OR		AL,080H
				OUT		060H,AL

POLL_INNER_LOOP:
				IN		AL,060H
				TEST	AL,1
				JE		POLL_INNER_LOOP

				LOOP	POLL_OUTER_LOOP

				POP		ECX
				RET
POLL_TIMER		ENDP

CODE			ENDS

				END
