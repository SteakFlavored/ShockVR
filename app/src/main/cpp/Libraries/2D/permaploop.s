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
;	File:		PermapLoop.s
;
;	Contains:	PowerPC assembly routine to handle lit/clut/transparent perspective mappers.
;
;	Written by:	Mark Adams
;
; 	PPCAsm PermapLoop.s -o PermapLoop.s.o
;

	; global variables
	
	; external functions
	
	csect
	
	
;---------------------------------------------------
; int opaque_lit_per_hscan_Loop_PPC();  C++ routine
;---------------------------------------------------
		EXPORT	.opaque_lit_per_hscan_Loop_PPC
;		EXPORT	.opaque_lit_per_hscan_Loop_PPC__FillPlPlPPUcPlPilPliiilPUciPUc
	
	.opaque_lit_per_hscan_Loop_PPC:
;	.opaque_lit_per_hscan_Loop_PPC__FillPlPlPPUcPlPilPliiilPUciPUc:
	
	stmw     r16,-68(SP)
	lwz      r17,56(SP)
	lwz      r16,60(SP)
	lwz      r18,64(SP)
	lwz      r19,68(SP)
	lwz      r20,72(SP)
	lwz      r21,76(SP)
	lwz      r22,80(SP)
	lwz      r23,84(SP)
	lwz      r24,88(SP)
	lwz      r29,0(r6)
	lwz      r28,0(r7)
	lwz      r25,0(r9)
	lwz      r27,0(r16)
	lwz      r26,0(r8)
	lwz      r30,0(r10)

	mtctr	 r3
	
Loop:
	srawi    r11,r29,16
	sraw     r12,r28,r19
	and      r11,r11,r18
	and      r12,r20,r12
	add      r31,r11,r12
	lbzx     r31,r22,r31
	rlwinm   r11,r27,24,16,23
	add      r11,r11,r24
	mr       r12,r26
	lbzx     r11,r11,r31
	mr       r31,r30
	add      r25,r25,r21
	srawi    r30,r25,16
	addi     r26,r26,1
	cmpw	 r31,r30
	stb      r11,0(r12)
	
	beq+	 Equal
	add		 r26,r26,r23
	
Equal:
	add      r29,r29,r4
	add      r28,r28,r5
	add      r27,r27,r17

	bdnz     Loop
	
	stw      r29,0(r6)
	stw      r28,0(r7)
	stw      r25,0(r9)
	stw      r27,0(r16)
	stw      r26,0(r8)
	stw      r30,0(r10)
	lmw      r16,-68(SP)
	blr
	
	
;---------------------------------------------------
; int opaque_lit_per_vscan_Loop_PPC();  C++ routine
;---------------------------------------------------
		EXPORT	.opaque_lit_per_vscan_Loop_PPC
;		EXPORT	.opaque_lit_per_vscan_Loop_PPC__FillPlPlPPUcPlPilPliiilPUciPUc
	
	.opaque_lit_per_vscan_Loop_PPC:
;	.opaque_lit_per_vscan_Loop_PPC__FillPlPlPPUcPlPilPliiilPUciPUc:

	stmw     r16,-64(SP)
	lwz      r17,56(SP)
	lwz      r16,60(SP)
	lwz      r18,64(SP)
	lwz      r19,68(SP)
	lwz      r20,72(SP)
	lwz      r21,76(SP)
	lwz      r22,80(SP)
	lwz      r23,84(SP)
	lwz      r24,88(SP)
	lwz      r28,0(r6)
	lwz      r27,0(r7)
	lwz      r25,0(r9)
	lwz      r26,0(r16)
	lwz      r29,0(r8)
	lwz      r30,0(r10)
	lis		 r15,-1
	
	mtctr	 r3
Loop2:
	sraw     r12,r27,r19
	srawi    r11,r28,16
	and      r12,r20,r12
	and      r31,r11,r18
	add      r31,r31,r12
	lbzx     r31,r22,r31
	rlwinm   r11,r26,24,16,23
	add      r11,r11,r24
	lbzx     r11,r11,r31
	stb      r11,0(r29)
	mr       r31,r30
	add      r25,r25,r21
	srawi    r30,r25,16
;	cmpw     r31,r30
	
;	beq+     Equal2
	sub      r12,r31,r30
	sub      r29,r29,r12
	
Equal2:
	add      r29,r29,r23
	add      r28,r28,r4
	add      r27,r27,r5
	add      r26,r26,r17

	bdnz     Loop2
	
	stw      r28,0(r6)
	stw      r27,0(r7)
	stw      r25,0(r9)
	stw      r26,0(r16)
	stw      r29,0(r8)
	stw      r30,0(r10)
	lmw      r16,-64(SP)
	blr
	

