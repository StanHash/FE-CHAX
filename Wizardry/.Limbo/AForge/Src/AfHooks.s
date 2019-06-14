
	.thumb @ do not forget! (well actually because of hashtag lyn I could forget and have it work just fine :) )

	.global AfHook_BGSI_Gc
	.type   AfHook_BGSI_Gc, function

AfHook_BGSI_Gc:
	@ call AfGc

	ldr r3, =AfGc
	bl  call_via_r3

	@ replaced (lol)
	pop {r3-r4}
	mov r8, r3
	mov r9, r4
	pop {r4-r7}
	pop {r3}

call_via_r3:
	bx  r3
