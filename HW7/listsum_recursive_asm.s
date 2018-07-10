/************************
 * listsum_recursive_asm.s
 *
 *   Authors: Nick Palumbo
 *   Date Created: 17 Apr. 2016
 *   Date Last Modified: 17 Apr. 2016
 *   Assignment: HW7
 *   Part of: HW7
 */

	.text
	.syntax unified
	.thumb
	.global	listsum_recursive_asm	
	.type listsum_recursive_asm, %function
listsum_recursive_asm:
/*	Your implementation goes here. */
	push {r4,lr}
	ldr r4, [r0]	// r4 = p->item
	ldr r2, [r0,4]  // r2 = p->next
	cmp r2, 0	// (p->next==NULL)
	beq end		// goto end if (r2==0)
	movs r0, r2	// r0 = r2
	bl listsum_recursive_asm	// keep looping	
	adds r4,r0	// r4 = r4 + r0
end:
	mov r0,r4 	// r0 = r4
	pop {r4,pc} 
	bx	lr
