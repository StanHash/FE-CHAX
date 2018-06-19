.macro gba_color r, g, b
	.short ((\r & 0x1F) | ((\g & 0x1F) << 5) | ((\b & 0x1F) << 10))
.endm

.global css_debug_pal
.type   css_debug_pal, %object

css_debug_pal:
	gba_color 0, 0, 0    @ 0 | none

	gba_color 30, 0, 0   @ 1 | red
	gba_color 0, 30, 0   @ 2 | green
	gba_color 0, 0, 30   @ 3 | blue
	
	gba_color 0, 30, 30  @ 4 | cyan
	gba_color 30, 0, 30  @ 5 | magenta
	gba_color 30, 30, 0  @ 6 | yellow

	gba_color 15, 0, 0   @ 7 | dark red
	gba_color 0, 15, 0   @ 8 | dark green
	gba_color 0, 0, 15   @ 9 | dark blue

	gba_color 0, 15, 15  @ A | dark cyan
	gba_color 15, 0, 15  @ B | dark magenta
	gba_color 15, 15, 0  @ C | dark yellow

	gba_color 10, 10, 10 @ D | dark grey
	gba_color 20, 20, 20 @ E | grey
	gba_color 30, 30, 30 @ F | white (actually still grey)
