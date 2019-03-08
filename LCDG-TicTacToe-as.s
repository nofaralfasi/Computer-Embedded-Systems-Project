

#include <p32xxxx.h>

	/* define all global symbols here */
.global main
.global num
	/* define all global symbols here */
   .data
num:
.byte 0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81 /*fan*/


	.text
	.set noreorder
    .ent main 

main:      
	    nop 	   /*for read next instruction*/
        andi       $s0,$s0,0 /* value for TRIS */
        
		/*init TRIS*/
		sw         $s0,TRISF
        sw         $s0,TRISE
        sw         $s0,TRISD 
        ori         $s0,$s0,0x4 /*rase bit 4 <-1 at TRISF for leds*/
        sw         $s0,PORTF
start:        
       li $s0,0x0   /*li=load imediatly*/               
       la $s4,num    /*load segment num*/

st_1:                         /*	 endless loop */
     lb   $3,($s4)    /*load value at adress*/
     jal print        #out_led_fan
     nop
     jal delay
     nop
     addi $s4,$s4,1   /*inc pointer*/
     addi $s0,$s0,1   /*inc counter*/
     bne $s0,8,st_1 /*if <8 do again*/
     nop
	
     j	  start	
	nop
.end main 

.ent delay
delay:
    
       andi $s1,$s1,0
       andi $s2,$s2,0
       ori $s2,$s2,0xffff
 x1:    addi $s1,$s1,1
        bne $s1,$s2,x1
       nop
	
		andi $s1,$s1,0
       andi $s2,$s2,0
       ori $s2,$s2,0xffff
 x2:    addi $s1,$s1,1
        bne $s1,$s2,x2
	nop

	/* return to caller */
	jr    $ra 
	nop
.end delay

.ent print
print:
        sw $3,PORTE 
        li    $2,0x10
        sw $2,PORTD
        li    $2,0x00
        sw $2,PORTD 
      	/* return to caller */
	jr    $ra
	nop
.end print

