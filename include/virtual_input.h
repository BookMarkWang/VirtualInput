#ifndef __VIRTUAL_INPUT_H__
#define __VIRTUAL_INPUT_H__

#include <stdint.h>
#include <linux/input.h>
#include "trace.h"

char* key_str[] = 
{
"RESERVED",              //KEY_RESERVED		0
"ESC",                   //KEY_ESC			1
"1",                     //KEY_1			    2
"2",                     //KEY_2			    3
"3",                     //KEY_3			    4
"4",                     //KEY_4			    5
"5",                     //KEY_5			    6
"6",                     //KEY_6			7
"7",                     //KEY_7			8
"8",                     //KEY_8			9
"9",                     //KEY_9			10
"0",                     //KEY_0			11
"-",                     //KEY_MINUS		12
"=",                     //KEY_EQUAL		13
"BACKSPACE",             //KEY_BACKSPACE		14
"TAB",                   //KEY_TAB			15
"Q",                     //KEY_Q			16
"W",                     //KEY_W			17
"E",                     //KEY_E			18
"R",                     //KEY_R			19
"T",                     //KEY_T			20
"Y",                     //KEY_Y			21
"U",                     //KEY_U			22
"I",                     //KEY_I			23
"O",                     //KEY_O			24
"P",                     //KEY_P			25
"{",                     //KEY_LEFTBRACE		26
"}",                     //KEY_RIGHTBRACE		27
"ENTER",                 //KEY_ENTER		28
"CTRL",                  //KEY_LEFTCTRL		29
"A",                     //KEY_A			30
"S",                     //KEY_S			31
"D",                     //KEY_D			32
"F",                     //KEY_F			33
"G",                     //KEY_G			34
"H",                     //KEY_H			35
"J",                     //KEY_J			36
"K",                     //KEY_K			37
"L",                     //KEY_L			38
";",                     //KEY_SEMICOLON		39
"'",                     //KEY_APOSTROPHE		40
"GRAVE",                 //KEY_GRAVE		41
"SHIFT",                 //KEY_LEFTSHIFT		42
"\\",                    //KEY_BACKSLASH		43
"Z",                     //KEY_Z			44
"X",                     //KEY_X			45
"C",                     //KEY_C			46
"V",                     //KEY_V			47
"B",                     //KEY_B			48
"N",                     //KEY_N			49
"M",                     //KEY_M			50
",",                     //KEY_COMMA		51
".",                     //KEY_DOT			52
"/",                     //KEY_SLASH		53
"RIGHTSHIFT",            //KEY_RIGHTSHIFT		54
"*",                     //KEY_KPASTERISK		55
"ALT",                   //KEY_LEFTALT		56
"SPACE",                 //KEY_SPACE		57
"CAPSLOCK",              //KEY_CAPSLOCK		58
"F1",                    //KEY_F1			59
"F2",                    //KEY_F2			60
"F3",                    //KEY_F3			61
"F4",                    //KEY_F4			62
"F5",                    //KEY_F5			63
"F6",                    //KEY_F6			64
"F7",                    //KEY_F7			65
"F8",                    //KEY_F8			66
"F9",                    //KEY_F9			67
"F10",                   //KEY_F10			68
"NUMLOCK",               //KEY_NUMLOCK		69
"SCROLLLOCK",            //KEY_SCROLLLOCK		70
"KP7",                   //KEY_KP7			71
"KP8",                   //KEY_KP8			72
"KP9",                   //KEY_KP9			73
"KPMINUS",               //KEY_KPMINUS		74
"KP4",                   //KEY_KP4			75
"KP5",                   //KEY_KP5			76
"KP6",                   //KEY_KP6			77
"KPPLUS",                //KEY_KPPLUS		78
"KP1",                   //KEY_KP1			79
"KP2",                   //KEY_KP2			80
"KP3",                   //KEY_KP3			81
"KP0",                   //KEY_KP0			82
"KPDOT",                 //KEY_KPDOT		83
"",                     
"ZENKAKUHANKAKU",        //KEY_ZENKAKUHANKAKU	85
"102ND",                 //KEY_102ND		86
"F11",                   //KEY_F11			87
"F12",                   //KEY_F12			88
"RO",                    //KEY_RO			89
"KATAKANA",              //KEY_KATAKANA		90
"HIRAGANA",              //KEY_HIRAGANA		91
"HENKAN",                //KEY_HENKAN		92
"KATAKANAHIRAGANA",      //KEY_KATAKANAHIRAGANA	93
"MUHENKAN",              //KEY_MUHENKAN		94
"KPJPCOMMA",             //KEY_KPJPCOMMA		95
"KPENTER",               //KEY_KPENTER		96
"RIGHTCTRL",             //KEY_RIGHTCTRL		97
"KPSLASH",               //KEY_KPSLASH		98
"SYSRQ",                 //KEY_SYSRQ		99
"RIGHTALT",              //KEY_RIGHTALT		100
"LINEFEED",              //KEY_LINEFEED		101
"HOME",                  //KEY_HOME		102
"UP",                    //KEY_UP			103
"PAGEUP",                //KEY_PAGEUP		104
"LEFT",                  //KEY_LEFT		105
"RIGHT",                 //KEY_RIGHT		106
"END",                   //KEY_END			107
"DOWN",                  //KEY_DOWN		108
"PAGEDOWN",              //KEY_PAGEDOWN		109
"INSERT",                //KEY_INSERT		110
"DELETE",                //KEY_DELETE		111
"MACRO",                 //KEY_MACRO		112
"MUTE",                  //KEY_MUTE		113
"VOLUMEDOWN",            //KEY_VOLUMEDOWN		114
"VOLUMEUP",              //KEY_VOLUMEUP		115
"POWER",                 //KEY_POWER		116	/* SC System Power Down */
"KPEQUAL",               //KEY_KPEQUAL		117
"KPPLUSMINUS",           //KEY_KPPLUSMINUS		118
"PAUSE",                 //KEY_PAUSE		119
"SCALE"                 //KEY_SCALE		120	/* AL Compiz Scale (Expose) */
};

//common
int input_simulate(int fd, uint16_t type, uint16_t code, int32_t value);
int input_capture(int fd, struct input_event *event);

//for virtual key
int key_str_simulate(int fd, char* key);
int key_simulate(int fd, uint16_t code);
int key_capture(int fd);

//for virtual mouse
int mouse_simulate(int fd, uint16_t code, int32_t value);


#endif //__VIRTUAL_INPUT_H__
