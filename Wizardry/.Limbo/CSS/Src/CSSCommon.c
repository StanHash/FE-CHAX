#include "CSS.h"

char* css_num2str(char* out, int number) {
	// TODO: better
	out[0] = '0' + number / 10;
	out[1] = '0' + number % 10;
	out[2] = 0;

	return (out[0] == '0') ? out+1 : out;
}

void css_text_append_number(struct TextHandle* text, int number) {
	char buf[0x10];

	if (number == 0xFF) {
		Text_AppendString(text, "--");
	} else {
		char* str = css_num2str(buf, number);
		Text_AppendString(text, str);
	}
}

void css_text_append_number_2digit(struct TextHandle* text, int number) {
	char buf[0x10];

	if (number == 0xFF) {
		Text_AppendString(text, "--");
	} else {
		css_num2str(buf, number);
		Text_AppendString(text, buf);
	}
}

void css_display_hp_exp_line(u16* bgOut) {
	struct TextHandle text;

	/* Drawing HP <MaxHp>/<CurHp> */

	Text_InitClear(&text, 6);

	Text_SetColorId(&text, 3);
	Text_AppendString(&text, "HP ");

	Text_SetColorId(&text, 0);
	css_text_append_number(&text, gpStatScreenUnit->curHP);

	Text_SetColorId(&text, 3);
	Text_AppendString(&text, "/");

	Text_SetColorId(&text, 0);
	css_text_append_number(&text, gpStatScreenUnit->maxHP);

	Text_Draw(&text, BG_LOCATED_TILE(bgOut, 0, 0));

	/* Drawing L<level> */
	// TODO: align right

	Text_InitClear(&text, 3);

	Text_SetColorId(&text, 3);
	Text_AppendString(&text, "L");

	Text_SetColorId(&text, 0);
	css_text_append_number_2digit(&text, gpStatScreenUnit->level);

	Text_Draw(&text, BG_LOCATED_TILE(bgOut, 7, 0));

	/* Drawing E<exp> */
	// TODO: align right

	Text_InitClear(&text, 4);

	Text_SetColorId(&text, 3);
	Text_AppendString(&text, "E");

	Text_SetColorId(&text, 0);
	css_text_append_number_2digit(&text, gpStatScreenUnit->exp);

	Text_Draw(&text, BG_LOCATED_TILE(bgOut, 10, 0));
}
