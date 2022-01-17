;
; Copyright (C) 2015-2018 Night Dive Studios, LLC.
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
;
;	File:		WallLitLoop1D.s
;
;	Contains:	PowerPC assembly routine to handle lit wall mapper 1D.
;
;	Written by:	Ken Cobb
;
; 	PPCAsm WallLitLoop1D.s -o WallLitLoop1D.s.o
;

	; global variables
	import grd_canvas

	; external functions
	import .fix_div_asm
	import .fix_mul_asm
	import .fix_mul_asm_safe

	toc
		tc grd_canvas[TC], grd_canvas

	csect

;---------------------------------------------------
; int32_t HandleWallLitLoop1D_PPC();  C++ routine
;---------------------------------------------------
		EXPORT	.HandleWallLitLoop1D_PPC
;		EXPORT	.HandleWallLitLoop1D_PPC__FP18grs_tmap_loop_infollllllPUcPlPUclUlUl

	.HandleWallLitLoop1D_PPC:
;	.HandleWallLitLoop1D_PPC__FP18grs_tmap_loop_infollllllPUcPlPUclUlUl:
		mflr     r0
		stmw     r13,-76(SP)
		stw      r0,8(SP)
		stwu     SP,-128(SP)
		mr       r25,r3

		mr		 r15,r4			; keep u in r15
		lwz      r14,188(SP)	; keep o_bits in r14

		mr       r28,r5
		mr       r29,r6
		mr       r23,r7
		mr       r24,r8

		mr		 r17,r9			; keep dy in r17

		mr       r21,r10
		lwz      r18,184(SP)
		lwz      r22,192(SP)
		lwz      r19,196(SP)
		lwz      r20,200(SP)

		lwz      r9,grd_canvas[TC](RTOC)
		lwz      r9,0(r9)
		lwz      r13,0(r9)

		;---------------------------
		; Top of "do" loop
		;---------------------------
	L_OuterLoop:
		lwz      r3,32(r25)
		addis    r3,r3,1
		subi     r3,r3,1
		clrrwi   r3,r3,16
		lwz      r4,68(r25)
		addis    r4,r4,1
		subi     r4,r4,1
		clrrwi   r4,r4,16
		sub.     r16,r4,r3
		ble      L_CheckDMinus
		lwz      r5,32(r25)
		lwz      r6,32(r25)
		addis    r6,r6,1
		subi     r6,r6,1
		clrrwi   r6,r6,16
		sub      r16,r6,r5

		;---------------------------
		; Calc dv and di
		;---------------------------
		lis      r3,256
		mr		 r4,r17
		bl       .fix_div_asm
		mr       r7,r3				; inv_dy in r7

		mr       r3,r23
		srawi    r4,r7,8
		bl       .fix_mul_asm_safe
		mr       r23,r3

		mr     	 r3,r24
		mr       r4,r7
		bl       .fix_mul_asm_safe
		addi     r3,r3,255
		srawi    r24,r3,8

		;---------------------------
		; Calc v and i
		;---------------------------
		mr       r3,r23
		mr       r4,r16
		bl       .fix_mul_asm
		add      r28,r28,r3

		mr       r3,r24
		mr       r4,r16
		bl       .fix_mul_asm
		add      r29,r29,r3

		;---------------------------
		; Calc y
		;---------------------------
		lwz      r7,32(r25)
		lwz      r8,68(r25)
		addis    r7,r7,1
		addis    r8,r8,1
		subi     r7,r7,1
		subi     r8,r8,1
		srawi    r7,r7,16
		srawi    r8,r8,16
#		extsh    r7,r7
#		extsh    r8,r8
		sub      r30,r8,r7
		cmpwi	 r30,0					; set condition code for count register check later

		;---------------------------
		; Setup p_dest
		;---------------------------
		lwz      r10,32(r25)

		addis    r10,r10,1
		subi     r10,r10,1
		srawi    r10,r10,16
#		extsh    r10,r10
		mullw    r10,r22,r10
		lwz      r11,4(r25)
		add      r10,r10,r11
		add      r27,r13,r10
		sub		 r27,r27,r22			; sub an extra gr_row for the stbux

		beq-	 L_LoopSkip				; if (r30) ctr is zero, skip loops

		cmpwi	 r24,256
		;---------------------------
		; Setup t_bits
		;---------------------------
		srawi    r3,r15,16
		cmpwi	 1,r24,-256		; moved for instruction flow
		add      r26,r14,r3


		bgt+	 Skip
		blt+	 1,Skip
		addi	 r29,r29,512
Skip:
		mtctr	 r30					; place y in CTR

	;--------------------------
	; case GRL_OPAQUE|GRL_LOG2
	;--------------------------
	L_InnerLoop:
		srawi    r11,r28,16				; fix_fint(v)
		rlwinm   r12,r29,24,16,23		; r12 = fix_light(i)

		slw      r11,r11,r20			; << t_wlog
		and      r31,r11,r19			; & t_mask

		lbzx     r3,r26,r31				; r3 = t_bits[k]
		add      r3,r3,r21				; r3 += g_ltab
		add      r29,r29,r24			; i += di  (do this while loading)

		lbzx     r12,r12,r3				; r12 = g_ltab[r12]+fix_light(i)
		add      r28,r28,r23			; v += dv (do this while loading)
		stbux    r12,r27,r22			; *p_dest = r12, p_dest += gr_row

		bdnz	 L_InnerLoop

	;--------------------------
	; Calc new left u, v, and i
	;--------------------------
	L_LoopSkip:
		lwz      r7,100(r25)
		lwz      r8,24(r25)
		add      r8,r8,r7
		stw      r8,24(r25)
		lwz      r9,36(r25)
		lwz      r10,52(r25)
		add      r31,r9,r10
		lwz      r11,40(r25)
		lwz      r12,56(r25)
		add      r30,r11,r12
		lwz      r3,60(r25)
		lwz      r4,44(r25)
		add      r4,r4,r3
		stw      r4,44(r25)

	;---------------------------
	; Calc inv_dy
	;---------------------------
		lis      r3,1
		lwz      r4,24(r25)
		bl       .fix_div_asm
		mr       r7,r3					; inv_dy in r7

	;---------------------------
	; Calc new u, v, and i
	;---------------------------
		mr       r3,r31
		mr       r4,r7
		bl       .fix_mul_asm_safe
		mr		 r15,r3

		mr       r3,r30
		mr       r4,r7
		bl       .fix_mul_asm_safe
		mr       r28,r3

		lwz      r3,44(r25)
		mr       r4,r7
		bl       .fix_mul_asm_safe
		mr	     r29,r3

		stw      r31,36(r25)
		stw      r30,40(r25)
		lwz      r6,72(r25)
		lwz      r8,88(r25)
		add      r31,r6,r8
		lwz      r8,76(r25)
		lwz      r9,92(r25)
		add      r30,r8,r9
		lwz      r10,96(r25)
		lwz      r11,80(r25)
		add      r11,r11,r10
		stw      r11,80(r25)

		mr       r3,r30
		mr       r4,r7
		bl       .fix_mul_asm_safe
		sub      r23,r3,r28

		lwz      r3,80(r25)
		mr       r4,r7
		bl       .fix_mul_asm_safe
		sub      r24,r3,r29

		cmpwi	 r24,256
		bgt+	 Skip2
		cmpwi	 r24,-256
		blt+	 Skip2
		addi	 r29,r29,512
Skip2:

		stw      r31,72(r25)		; tli->right.u = k
		stw      r30,76(r25)		; tli->right.v = y

		lwz      r3,48(r25)			; tli->left.y+=tli->left.dy
		lwz      r4,32(r25)
		add      r4,r4,r3
		stw      r4,32(r25)

		lwz      r5,84(r25)			; tli->right.y+=tli->right.dy
		lwz      r6,68(r25)
		add      r6,r6,r5
		stw      r6,68(r25)

		lwz      r7,32(r25)			; r7 = tli->left.y
		lwz      r8,68(r25)			; r8 = tli->right.y
		sub      r17,r8,r7			; calc dy (r8-r7)

		lwz      r10,0(r25)			; (tli->n)--
		addic.   r10,r10,-1			; set condition register now
		stw      r10,0(r25)

		lwz      r9,4(r25)			; (tli->x)++
		addi     r9,r9,1
		stw      r9,4(r25)

		bgt+     L_OuterLoop			; is r10 > 0? (yes, most of the time)

		; return FALSE
		li       r3,0
L_Done:
		lwz      r0,136(SP)
		addi     SP,SP,128
		mtlr     r0
		lmw      r13,-76(SP)
		blr

L_CheckDMinus:
		cmpwi    r16,0
		bge+     L_LoopSkip

		; return TRUE
		li       r3,1
		b        L_Done


;---------------------------------------------------
; int32_t HandleWallLoop1D_PPC();  C++ routine
;---------------------------------------------------
		EXPORT	.HandleWallLoop1D_PPC
;		EXPORT	.HandleWallLoop1D_PPC__FP18grs_tmap_loop_infollllPUcPlPUclUlUl

	.HandleWallLoop1D_PPC:
;	.HandleWallLoop1D_PPC__FP18grs_tmap_loop_infollllPUcPlPUclUlUl:
		mflr     r0
		stmw     r13,-76(SP)
		stw      r0,8(SP)
		stwu     SP,-128(SP)

		mr       r25,r3
		mr       r15,r4
		mr       r26,r5
		mr       r21,r6
		mr       r16,r7
		mr       r17,r8
		lwz      r20,184(SP)
		lwz      r18,188(SP)
		lwz      r19,192(SP)
		lwz      r11,0(r25)
		lwz      r4,4(r25)
		lwz      r12,24(r25)
		lwz      r27,32(r25)
		lwz		 r14,36(r25)
		lwz		 r13,40(r25)
		lwz      r24,68(r25)

		lwz      r5,grd_canvas[TC](RTOC)
		lwz      r5,0(r5)
		lwz      r22,0(r5)
		add		 r22,r22,r4			; grd_bits = grd_bm.bits + tli->x

		add      r4,r4,r11
		stw      r4,4(r25)			; tli->x += tli->n

		;---------------------------
		; Top of "do" loop
		;---------------------------
OuterLoop:
		addis    r7,r24,1
		addis    r6,r27,1
		subi     r6,r6,1
		subi     r7,r7,1
		clrrwi   r6,r6,16
		clrrwi   r7,r7,16
		sub.     r31,r7,r6
		ble      CheckDMinus

		addis    r8,r27,1
		subi     r8,r8,1
		clrrwi   r8,r8,16
		sub      r31,r8,r27

		;---------------------------
		; Calc dv & v
		;---------------------------
		mr       r3,r21
		mr       r4,r16
		bl       .fix_div_asm
		mr       r21,r3

		mr       r4,r31
		bl       .fix_mul_asm
		add      r26,r26,r3

		;---------------------------
		; Calc p_dest
		;---------------------------
		addis    r3,r27,1
		subi     r3,r3,1
		srawi    r3,r3,16
#		extsh    r3,r3

		mullw    r3,r20,r3
		add      r29,r22,r3
		sub		 r29,r29,r20			; sub an extra gr_row for the stbux

		;---------------------------
		; Calc y
		;---------------------------
		addis    r4,r27,1
		addis    r3,r24,1
		subi     r4,r4,1
		subi     r3,r3,1
		srawi    r4,r4,16
		srawi    r3,r3,16
#		extsh    r4,r4
#		extsh    r3,r3
		sub.     r30,r3,r4

		;---------------------------
		; Calc t_bits
		;---------------------------
		srawi    r4,r15,16
		add      r28,r10,r4

		beq		 SkipLoop			; branch if counter is 0
		mtctr	 r30

InnerLoop:
		srawi    r5,r26,16
		slw      r5,r5,r19
		and      r31,r5,r18		; k = ((fix_fint(v)<<t_wlog))&t_mask

		lbzx     r6,r28,r31		; t_bits[k]
		lbzx     r6,r17,r6		; t_clut[t_bits[k]]
		add      r26,r26,r21	; v+=dv
		stbux    r6,r29,r20		; *p_dest = t_clut[t_bits[k]], p+dest+=gr_row
		bdnz     InnerLoop

SkipLoop:
		lwz      r7,100(r25)
		add      r12,r12,r7		; tli->w+=tli->dw;

		; figure out new left u & v
		lwz      r4,52(r25)
		add      r14,r14,r4
		lwz      r3,56(r25)
		add      r13,r13,r3
		lis      r3,1
		mr       r4,r12
		bl       .fix_div_asm
		mr       r23,r3
		mr       r3,r14
		mr       r4,r23
		bl       .fix_mul_asm_safe
		mr       r15,r3
		mr       r3,r13
		mr       r4,r23
		bl       .fix_mul_asm_safe
		mr       r26,r3

		; figure out new right u & v
		lwz      r3,88(r25)
		lwz      r4,72(r25)
		add      r4,r4,r3
		stw      r4,72(r25)
		lwz      r5,76(r25)
		lwz      r6,92(r25)
		add      r30,r5,r6
		mr       r3,r30
		mr       r4,r23
		bl       .fix_mul_asm_safe
		sub      r21,r3,r26
		stw      r30,76(r25)

		lwz      r7,48(r25)
		add      r27,r27,r7			; ly+=tli->left.dy
		lwz      r8,84(r25)
		add      r24,r24,r8			; ry+=tli->right.dy
		addic.   r11,r11,-1			; --(tli->n)
		sub      r16,r24,r27		; dy=ry-ly
		addi     r22,r22,1			; grd_bits++
		bgt-     OuterLoop

		stw      r11,0(r25)			; store out tli stuff
		stw      r12,24(r25)
		stw      r27,32(r25)
		stw		 r14,36(r25)
		stw		 r13,40(r25)
		stw      r24,68(r25)

		li       r3,0				; return 0 (ok)

Done:
		lwz      r0,136(SP)
		addi     SP,SP,128
		mtlr     r0
		lmw      r13,-76(SP)
		blr

CheckDMinus:
		cmpwi    r31,0
		bge+     SkipLoop
		li       r3,1
		b      	 Done
