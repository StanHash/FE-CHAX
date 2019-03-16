#include "CSS.h"

char* css_num2str(char* out, int number) {
	// TODO: better
	out[0] = '0' + Div(number, 10);
	out[1] = '0' + Mod(number, 10);
	out[2] = 0;

	return (out[0] == '0') ? out+1 : out;
}

void css_text_append_number(struct TextHandle* text, int number) {
	char buf[0x10];

	if (number == 0xFF) {
		Text_DrawString(text, "--");
	} else {
		char* str = css_num2str(buf, number);
		Text_DrawString(text, str);
	}
}

void css_text_append_number_2digit(struct TextHandle* text, int number) {
	char buf[0x10];

	if (number == 0xFF) {
		Text_DrawString(text, "--");
	} else {
		css_num2str(buf, number);
		Text_DrawString(text, buf);
	}
}

void css_display_hp_exp_line(u16* bgOut) {
	struct TextHandle text;

	/* Drawing HP <MaxHp>/<CurHp> */

	Text_InitClear(&text, 6);

	Text_SetColorId(&text, 3);
	Text_DrawString(&text, "HP ");

	Text_SetColorId(&text, 0);
	css_text_append_number(&text, gpStatScreenUnit->curHP);

	Text_SetColorId(&text, 3);
	Text_DrawString(&text, "/");

	Text_SetColorId(&text, 0);
	css_text_append_number(&text, gpStatScreenUnit->maxHP);

	Text_Display(&text, BG_LOCATED_TILE(bgOut, 0, 0));

	/* Drawing L<level> */

	Text_InitClear(&text, 3);

	Text_SetXCursor(&text, 22 - Text_GetStringTextWidth("L00"));

	Text_SetColorId(&text, 3);
	Text_DrawString(&text, "L");

	Text_Advance(&text, 1);

	Text_SetColorId(&text, 0);
	css_text_append_number_2digit(&text, gpStatScreenUnit->level);

	Text_Display(&text, BG_LOCATED_TILE(bgOut, 7, 0));

	/* Drawing E<exp> */
	// TODO: align right

	Text_InitClear(&text, 4);

	Text_SetXCursor(&text, 22 - Text_GetStringTextWidth("E00"));

	Text_SetColorId(&text, 3);
	Text_DrawString(&text, "E");

	Text_Advance(&text, 1);

	Text_SetColorId(&text, 0);
	css_text_append_number_2digit(&text, gpStatScreenUnit->exp);

	Text_Display(&text, BG_LOCATED_TILE(bgOut, 10, 0));
}
