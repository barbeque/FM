#include <linux/input.h>
#include "fm77avkeymap.h"
#include "../FM77AVKeyboard/fm77avkey.h"

FM77AVKeyMap::FM77AVKeyMap()
{
	MakeUS101KeyMap();
}

void FM77AVKeyMap::MakeUS101KeyMap(void)
{
	map.clear();
	map[KEY_SPACE]=           AVKEY_MID_SPACE;
	map[KEY_0]=               AVKEY_0;
	map[KEY_1]=               AVKEY_1;
	map[KEY_2]=               AVKEY_2;
	map[KEY_3]=               AVKEY_3;
	map[KEY_4]=               AVKEY_4;
	map[KEY_5]=               AVKEY_5;
	map[KEY_6]=               AVKEY_6;
	map[KEY_7]=               AVKEY_7;
	map[KEY_8]=               AVKEY_8;
	map[KEY_9]=               AVKEY_9;
	map[KEY_A]=               AVKEY_A;
	map[KEY_B]=               AVKEY_B;
	map[KEY_C]=               AVKEY_C;
	map[KEY_D]=               AVKEY_D;
	map[KEY_E]=               AVKEY_E;
	map[KEY_F]=               AVKEY_F;
	map[KEY_G]=               AVKEY_G;
	map[KEY_H]=               AVKEY_H;
	map[KEY_I]=               AVKEY_I;
	map[KEY_J]=               AVKEY_J;
	map[KEY_K]=               AVKEY_K;
	map[KEY_L]=               AVKEY_L;
	map[KEY_M]=               AVKEY_M;
	map[KEY_N]=               AVKEY_N;
	map[KEY_O]=               AVKEY_O;
	map[KEY_P]=               AVKEY_P;
	map[KEY_Q]=               AVKEY_Q;
	map[KEY_R]=               AVKEY_R;
	map[KEY_S]=               AVKEY_S;
	map[KEY_T]=               AVKEY_T;
	map[KEY_U]=               AVKEY_U;
	map[KEY_V]=               AVKEY_V;
	map[KEY_W]=               AVKEY_W;
	map[KEY_X]=               AVKEY_X;
	map[KEY_Y]=               AVKEY_Y;
	map[KEY_Z]=               AVKEY_Z;
	map[KEY_ESC]=             AVKEY_BREAK;
	map[KEY_F1]=              AVKEY_PF1;
	map[KEY_F2]=              AVKEY_PF2;
	map[KEY_F3]=              AVKEY_PF3;
	map[KEY_F4]=              AVKEY_PF4;
	map[KEY_F5]=              AVKEY_PF5;
	map[KEY_F6]=              AVKEY_PF6;
	map[KEY_F7]=              AVKEY_PF7;
	map[KEY_F8]=              AVKEY_PF8;
	map[KEY_F9]=              AVKEY_PF9;
	map[KEY_F10]=             AVKEY_PF10;
	map[KEY_F11]=             AVKEY_LEFT_SPACE;
	map[KEY_F12]=             AVKEY_RIGHT_SPACE;
	map[KEY_PRINT]=           AVKEY_NULL;
	map[KEY_CAPSLOCK]=        AVKEY_CAPS;
	map[KEY_SCROLLLOCK]=      AVKEY_NULL;
	map[KEY_BREAK]=           AVKEY_BREAK;
	map[KEY_PAUSE]=           AVKEY_BREAK;
	map[KEY_BACKSPACE]=       AVKEY_BACKSPACE;
	map[KEY_TAB]=             AVKEY_TAB;
	map[KEY_ENTER]=           AVKEY_RETURN;
	map[KEY_INSERT]=          AVKEY_INS;
	map[KEY_DELETE]=          AVKEY_DEL;
	map[KEY_HOME]=            AVKEY_HOME;
	map[KEY_END]=             AVKEY_DUP;
	map[KEY_PAGEUP]=          AVKEY_NULL;
	map[KEY_PAGEDOWN]=        AVKEY_NULL;
	map[KEY_UP]=              AVKEY_UP;
	map[KEY_DOWN]=            AVKEY_DOWN;
	map[KEY_LEFT]=            AVKEY_LEFT;
	map[KEY_RIGHT]=           AVKEY_RIGHT;
	map[KEY_NUMLOCK]=         AVKEY_NULL;
	map[KEY_GRAVE]=           AVKEY_ESC;
	map[KEY_MINUS]=           AVKEY_MINUS;
	map[KEY_EQUAL]=           AVKEY_HAT;
	map[KEY_LEFTBRACE]=       AVKEY_AT;
	map[KEY_RIGHTBRACE]=      AVKEY_LEFT_SQUARE_BRACKET;
	map[KEY_BACKSLASH]=       AVKEY_YEN;
	map[KEY_SEMICOLON]=       AVKEY_SEMICOLON;
	map[KEY_APOSTROPHE]=      AVKEY_COLON;
	map[KEY_COMMA]=           AVKEY_COMMA;
	map[KEY_DOT]=             AVKEY_DOT;
	map[KEY_SLASH]=           AVKEY_SLASH;
	map[KEY_KP0]=             AVKEY_NUM_0;
	map[KEY_KP1]=             AVKEY_NUM_1;
	map[KEY_KP2]=             AVKEY_NUM_2;
	map[KEY_KP3]=             AVKEY_NUM_3;
	map[KEY_KP4]=             AVKEY_NUM_4;
	map[KEY_KP5]=             AVKEY_NUM_5;
	map[KEY_KP6]=             AVKEY_NUM_6;
	map[KEY_KP7]=             AVKEY_NUM_7;
	map[KEY_KP8]=             AVKEY_NUM_8;
	map[KEY_KP9]=             AVKEY_NUM_9;
	map[KEY_KPDOT]=           AVKEY_NUM_DOT;
	map[KEY_KPSLASH]=         AVKEY_NUM_SLASH;
	map[KEY_KPASTERISK]=      AVKEY_NUM_STAR;
	map[KEY_KPMINUS]=         AVKEY_NUM_MINUS;
	map[KEY_KPPLUS]=          AVKEY_NUM_PLUS;
	map[KEY_KPENTER]=         AVKEY_NUM_RETURN;

	map[KEY_HENKAN]=          AVKEY_LEFT_SPACE;
	map[KEY_MUHENKAN]=        AVKEY_RIGHT_SPACE;

	map[KEY_LEFTCTRL]=        AVKEY_CTRL;
	map[KEY_RIGHTCTRL]=       AVKEY_GRAPH;
	map[KEY_LEFTSHIFT]=       AVKEY_LEFT_SHIFT;
	map[KEY_RIGHTSHIFT]=      AVKEY_RIGHT_SHIFT;
	map[KEY_LEFTALT]=         AVKEY_LEFT_SPACE;
	map[KEY_RIGHTALT]=        AVKEY_RIGHT_SPACE;
}

/* void FM77AVKeyMap::MakeXXXMap(void)
{
	map[FSKEY_SPACE]=           AVKEY_
	map[FSKEY_0]=               AVKEY_
	map[FSKEY_1]=               AVKEY_
	map[FSKEY_2]=               AVKEY_
	map[FSKEY_3]=               AVKEY_
	map[FSKEY_4]=               AVKEY_
	map[FSKEY_5]=               AVKEY_
	map[FSKEY_6]=               AVKEY_
	map[FSKEY_7]=               AVKEY_
	map[FSKEY_8]=               AVKEY_
	map[FSKEY_9]=               AVKEY_
	map[FSKEY_A]=               AVKEY_
	map[FSKEY_B]=               AVKEY_
	map[FSKEY_C]=               AVKEY_
	map[FSKEY_D]=               AVKEY_
	map[FSKEY_E]=               AVKEY_
	map[FSKEY_F]=               AVKEY_
	map[FSKEY_G]=               AVKEY_
	map[FSKEY_H]=               AVKEY_
	map[FSKEY_I]=               AVKEY_
	map[FSKEY_J]=               AVKEY_
	map[FSKEY_K]=               AVKEY_
	map[FSKEY_L]=               AVKEY_
	map[FSKEY_M]=               AVKEY_
	map[FSKEY_N]=               AVKEY_
	map[FSKEY_O]=               AVKEY_
	map[FSKEY_P]=               AVKEY_
	map[FSKEY_Q]=               AVKEY_
	map[FSKEY_R]=               AVKEY_
	map[FSKEY_S]=               AVKEY_
	map[FSKEY_T]=               AVKEY_
	map[FSKEY_U]=               AVKEY_
	map[FSKEY_V]=               AVKEY_
	map[FSKEY_W]=               AVKEY_
	map[FSKEY_X]=               AVKEY_
	map[FSKEY_Y]=               AVKEY_
	map[FSKEY_Z]=               AVKEY_
	map[FSKEY_ESC]=             AVKEY_
	map[FSKEY_F1]=              AVKEY_
	map[FSKEY_F2]=              AVKEY_
	map[FSKEY_F3]=              AVKEY_
	map[FSKEY_F4]=              AVKEY_
	map[FSKEY_F5]=              AVKEY_
	map[FSKEY_F6]=              AVKEY_
	map[FSKEY_F7]=              AVKEY_
	map[FSKEY_F8]=              AVKEY_
	map[FSKEY_F9]=              AVKEY_
	map[FSKEY_F10]=             AVKEY_
	map[FSKEY_F11]=             AVKEY_
	map[FSKEY_F12]=             AVKEY_
	map[FSKEY_PRINTSCRN]=       AVKEY_
	map[FSKEY_CAPSLOCK]=        AVKEY_
	map[FSKEY_SCROLLLOCK]=      AVKEY_
	map[FSKEY_PAUSEBREAK]=      AVKEY_
	map[FSKEY_BS]=              AVKEY_
	map[FSKEY_TAB]=             AVKEY_
	map[FSKEY_ENTER]=           AVKEY_
	map[FSKEY_SHIFT]=           AVKEY_
	map[FSKEY_CTRL]=            AVKEY_
	map[FSKEY_ALT]=             AVKEY_
	map[FSKEY_INS]=             AVKEY_
	map[FSKEY_DEL]=             AVKEY_
	map[FSKEY_HOME]=            AVKEY_
	map[FSKEY_END]=             AVKEY_
	map[FSKEY_PAGEUP]=          AVKEY_
	map[FSKEY_PAGEDOWN]=        AVKEY_
	map[FSKEY_UP]=              AVKEY_
	map[FSKEY_DOWN]=            AVKEY_
	map[FSKEY_LEFT]=            AVKEY_
	map[FSKEY_RIGHT]=           AVKEY_
	map[FSKEY_NUMLOCK]=         AVKEY_
	map[FSKEY_TILDA]=           AVKEY_
	map[FSKEY_MINUS]=           AVKEY_
	map[FSKEY_PLUS]=            AVKEY_
	map[FSKEY_LBRACKET]=        AVKEY_
	map[FSKEY_RBRACKET]=        AVKEY_
	map[FSKEY_BACKSLASH]=       AVKEY_
	map[FSKEY_SEMICOLON]=       AVKEY_
	map[FSKEY_SINGLEQUOTE]=     AVKEY_
	map[FSKEY_COMMA]=           AVKEY_
	map[FSKEY_DOT]=             AVKEY_
	map[FSKEY_SLASH]=           AVKEY_
	map[FSKEY_TEN0]=            AVKEY_
	map[FSKEY_TEN1]=            AVKEY_
	map[FSKEY_TEN2]=            AVKEY_
	map[FSKEY_TEN3]=            AVKEY_
	map[FSKEY_TEN4]=            AVKEY_
	map[FSKEY_TEN5]=            AVKEY_
	map[FSKEY_TEN6]=            AVKEY_
	map[FSKEY_TEN7]=            AVKEY_
	map[FSKEY_TEN8]=            AVKEY_
	map[FSKEY_TEN9]=            AVKEY_
	map[FSKEY_TENDOT]=          AVKEY_
	map[FSKEY_TENSLASH]=        AVKEY_
	map[FSKEY_TENSTAR]=         AVKEY_
	map[FSKEY_TENMINUS]=        AVKEY_
	map[FSKEY_TENPLUS]=         AVKEY_
	map[FSKEY_TENENTER]=        AVKEY_
	map[FSKEY_WHEELUP]=         AVKEY_
	map[FSKEY_WHEELDOWN]=       AVKEY_

	map[FSKEY_CONVERT]=         AVKEY_
	map[FSKEY_NONCONVERT]=      AVKEY_

	map[FSKEY_LEFT_CTRL]=       AVKEY_
	map[FSKEY_RIGHT_CTRL]=      AVKEY_
	map[FSKEY_LEFT_SHIFT]=      AVKEY_
	map[FSKEY_RIGHT_SHIFT]=     AVKEY_
	map[FSKEY_LEFT_ALT]=        AVKEY_
	map[FSKEY_RIGHT_ALT]=       AVKEY_
} */


////////////////////////////////////////////////////////////



