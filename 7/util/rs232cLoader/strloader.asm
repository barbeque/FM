					; To be stored inside F-BASIC string.
					; The server will escape with $20
					;    If b==$20, take COM of the next byte.

					; The server will identify where to encode by looking at the last two bytes of this binary.

					; To be used as:
					; 10 OPEN "I",#1,"COM0:(F8N1)"
					; 20 LINE INPUT #1,A$
					; 30 CLOSE
					; 40 EXEC VARPTR(A$)

					; To make it possible, the length sent from the server needs to be exactly 0x7E bytes.
					; 0x7E is JMP.

					; F-BASIC string (A$) is stored as:
					;   [VARPTR(A$)]    Number of bytes
					;   [VARPTR(A$)+1]  String Pointer Higher Byte
					;   [VARPTR(A$)+2]  String Pointer Lower Byte
					; To jump to the instruction stored in the string, the first byte needs to be 0x7E.

					; The server will encode the binary as follows and then send to FM-7.
					;
					; 	std::vector <unsigned char> toSend;
					; 	for(auto c : binary)
					; 	{
					; 		if(0x20<c)
					; 		{
					; 			toSend.push_back(c);
					; 		}
					; 		else
					; 		{
					; 			toSend.push_back(0x20);
					; 			toSend.push_back((~c)&0xFF);
					; 		}
					; 	}
					; 	while(toSend.size()<0x7E)
					; 	{
					; 		toSend.push_back('0');
					; 	}
					;	toSend.push_back(0x0d);
					;	toSend.push_back(0x0a);

INSTALLER_ADDR		EQU		$6000


PROGRAM_BEGIN
					ORG		$2000

					; IO_RS232C_CMD		$FD07	(7,U)
					; IO_RS232C_DATA	$FD06	(6,U)
					; IO_RS232C_ENABLE	$FD0C	($0C,U)	FM77AV20/40 and newer only
					; IO_RS232C_MODE	$FD0B	($0B,U)	FM77AV20/40 and newer only

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

					; Decoder part
					; No byte can be below or equal to #$20 unsigned.


					LEAX	END_OF_DECODER+$2F,PCR
					LEAX	-$2F,X
					LEAU	,X
					LDB		#END_OF_PROGRAM-END_OF_DECODER

DECODE_LOOP			LDA		#$DF		; #$DF=COM($20)
					COM		,X

					FCB		$8C			; =CMPX
DECODE_ESCAPE		CMPA	,X+			; Comes here only if A=0. Next byte is always $20+. Next SUBA ,X wont be zero.
					SUBA	,X
					BEQ		DECODE_ESCAPE

					LDA		,X+
					COMA
					STA		,U+

					DECB
					BNE		DECODE_LOOP

END_OF_DECODER

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




LOADER_START

					ORCC	#$50

					PULS	U	; Now EXEC VARPTR(A$) relies on JSR ($BD) instead of JMP ($7E).  Need to forget one PC.

					LDU		#IO_RS232C_COM0_DATA
					BSR		RESET_RS232C
					LDU		#IO_RS232C_COM1_DATA
					BSR		RESET_RS232C

					; F-BASIC ROM does RS232C presence check by:
					;   Writing command $10 (Error Reset), and then
					;   Reading status word to see if it $FF.
					; Presumably, if RS232C is not present, I/O read will return $FF
					; RESET_RS232C will send Error Reset command in the end.
					LDA		1,U
					INCA		; Zero if I/O Read returns $FF
					BEQ		USE_COM0
USE_COM1			; U is already #IO_RS232C_COM1_DATA
					LDA		#'a'
					STA		YAMAKAWA+7,PCR
					BRA		SEND_REQUEST

USE_COM0			LDU		#IO_RS232C_COM0_DATA



SEND_REQUEST
					LEAX	YAMAKAWA,PCR

SEND_REQ_CMD_LOOP	INCA
					BNE		SEND_REQ_CMD_LOOP

					LDA		,X+
					BMI		SEND_REQ_CMD_END

WAIT_TXRDY			LDB		1,U
					LSRB
					BCC		WAIT_TXRDY
					STA		,U
					BRA		SEND_REQ_CMD_LOOP
SEND_REQ_CMD_END


					LDX		#INSTALLER_ADDR

					CLRB
LOAD_LOOP			LDA		#2
					ANDA	1,U
					BEQ		LOAD_LOOP
					LDA		,U				; 1 byte shorter than "LDA IO_RS232C_DATA"
					STA		,X+
					DECB
					BNE		LOAD_LOOP

					JMP		INSTALLER_ADDR



RESET_RS232C
					LEAX	RS232C_RESET_CMD,PCR
RS232C_RESET_LOOP	MUL
					LDA		,X+
					COMA
					STA		1,U
					BPL		RS232C_RESET_LOOP
					RTS

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Keep "YAMAKAWA" immediately before RS232C_RESET_CMD.
YAMAKAWA			FCB 	"YAMAKAWA"				 ; Subsequent $FF will be a stopper.
RS232C_RESET_CMD	FCB		$FF,$FF,$FF,$BF,$B1,$48  ; COM and write to FD07
; Last byte $48 -> Complement = 0b10110111 -> Bit 4 Error Reset is on
; After RESET, error must be clear.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

END_OF_PROGRAM
