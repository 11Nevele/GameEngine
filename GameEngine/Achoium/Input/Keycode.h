#pragma once
/* Printable keys */
#define AC_KEY_SPACE              32
#define AC_KEY_APOSTROPHE         39  /* ' */
#define AC_KEY_COMMA              44  /* , */
#define AC_KEY_MINUS              45  /* - */
#define AC_KEY_PERIOD             46  /* . */
#define AC_KEY_SLASH              47  /* / */
#define AC_KEY_0                  48
#define AC_KEY_1                  49
#define AC_KEY_2                  50
#define AC_KEY_3                  51
#define AC_KEY_4                  52
#define AC_KEY_5                  53
#define AC_KEY_6                  54
#define AC_KEY_7                  55
#define AC_KEY_8                  56
#define AC_KEY_9                  57
#define AC_KEY_SEMICOLON          59  /* ; */
#define AC_KEY_EQUAL              61  /* = */
#define AC_KEY_A                  65
#define AC_KEY_B                  66
#define AC_KEY_C                  67
#define AC_KEY_D                  68
#define AC_KEY_E                  69
#define AC_KEY_F                  70
#define AC_KEY_G                  71
#define AC_KEY_H                  72
#define AC_KEY_I                  73
#define AC_KEY_J                  74
#define AC_KEY_K                  75
#define AC_KEY_L                  76
#define AC_KEY_M                  77
#define AC_KEY_N                  78
#define AC_KEY_O                  79
#define AC_KEY_P                  80
#define AC_KEY_Q                  81
#define AC_KEY_R                  82
#define AC_KEY_S                  83
#define AC_KEY_T                  84
#define AC_KEY_U                  85
#define AC_KEY_V                  86
#define AC_KEY_W                  87
#define AC_KEY_X                  88
#define AC_KEY_Y                  89
#define AC_KEY_Z                  90
#define AC_KEY_LEFT_BRACKET       91  /* [ */
#define AC_KEY_BACKSLASH          92  /* \ */
#define AC_KEY_RIGHT_BRACKET      93  /* ] */
#define AC_KEY_GRAVE_ACCENT       96  /* ` */
#define AC_KEY_WORLD_1            161 /* non-US #1 */
#define AC_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define AC_KEY_ESCAPE             256
#define AC_KEY_ENTER              257
#define AC_KEY_TAB                258
#define AC_KEY_BACKSPACE          259
#define AC_KEY_INSERT             260
#define AC_KEY_DELETE             261
#define AC_KEY_RIGHT              262
#define AC_KEY_LEFT               263
#define AC_KEY_DOWN               264
#define AC_KEY_UP                 265
#define AC_KEY_PAGE_UP            266
#define AC_KEY_PAGE_DOWN          267
#define AC_KEY_HOME               268
#define AC_KEY_END                269
#define AC_KEY_CAPS_LOCK          280
#define AC_KEY_SCROLL_LOCK        281
#define AC_KEY_NUM_LOCK           282
#define AC_KEY_PRINT_SCREEN       283
#define AC_KEY_PAUSE              284
#define AC_KEY_F1                 290
#define AC_KEY_F2                 291
#define AC_KEY_F3                 292
#define AC_KEY_F4                 293
#define AC_KEY_F5                 294
#define AC_KEY_F6                 295
#define AC_KEY_F7                 296
#define AC_KEY_F8                 297
#define AC_KEY_F9                 298
#define AC_KEY_F10                299
#define AC_KEY_F11                300
#define AC_KEY_F12                301
#define AC_KEY_F13                302
#define AC_KEY_F14                303
#define AC_KEY_F15                304
#define AC_KEY_F16                305
#define AC_KEY_F17                306
#define AC_KEY_F18                307
#define AC_KEY_F19                308
#define AC_KEY_F20                309
#define AC_KEY_F21                310
#define AC_KEY_F22                311
#define AC_KEY_F23                312
#define AC_KEY_F24                313
#define AC_KEY_F25                314
#define AC_KEY_KP_0               320
#define AC_KEY_KP_1               321
#define AC_KEY_KP_2               322
#define AC_KEY_KP_3               323
#define AC_KEY_KP_4               324
#define AC_KEY_KP_5               325
#define AC_KEY_KP_6               326
#define AC_KEY_KP_7               327
#define AC_KEY_KP_8               328
#define AC_KEY_KP_9               329
#define AC_KEY_KP_DECIMAL         330
#define AC_KEY_KP_DIVIDE          331
#define AC_KEY_KP_MULTIPLY        332
#define AC_KEY_KP_SUBTRACT        333
#define AC_KEY_KP_ADD             334
#define AC_KEY_KP_ENTER           335
#define AC_KEY_KP_EQUAL           336
#define AC_KEY_LEFT_SHIFT         340
#define AC_KEY_LEFT_CONTROL       341
#define AC_KEY_LEFT_ALT           342
#define AC_KEY_LEFT_SUPER         343
#define AC_KEY_RIGHT_SHIFT        344
#define AC_KEY_RIGHT_CONTROL      345
#define AC_KEY_RIGHT_ALT          346
#define AC_KEY_RIGHT_SUPER        347
#define AC_KEY_MENU               348

//A list that contains all the keycodes
#define AC_KEY_COUNT              120

static int keycodeList[120] = {
    AC_KEY_SPACE, AC_KEY_APOSTROPHE, AC_KEY_COMMA, AC_KEY_MINUS, AC_KEY_PERIOD, AC_KEY_SLASH,
    AC_KEY_0, AC_KEY_1, AC_KEY_2, AC_KEY_3, AC_KEY_4, AC_KEY_5, AC_KEY_6, AC_KEY_7, AC_KEY_8, AC_KEY_9,
    AC_KEY_SEMICOLON, AC_KEY_EQUAL, AC_KEY_A, AC_KEY_B, AC_KEY_C, AC_KEY_D, AC_KEY_E, AC_KEY_F, AC_KEY_G, AC_KEY_H, AC_KEY_I, AC_KEY_J, AC_KEY_K, AC_KEY_L, AC_KEY_M, AC_KEY_N, AC_KEY_O, AC_KEY_P, AC_KEY_Q, AC_KEY_R, AC_KEY_S, AC_KEY_T, AC_KEY_U, AC_KEY_V, AC_KEY_W, AC_KEY_X, AC_KEY_Y, AC_KEY_Z,
    AC_KEY_LEFT_BRACKET, AC_KEY_BACKSLASH, AC_KEY_RIGHT_BRACKET, AC_KEY_GRAVE_ACCENT, AC_KEY_WORLD_1, AC_KEY_WORLD_2,
    AC_KEY_ESCAPE, AC_KEY_ENTER, AC_KEY_TAB, AC_KEY_BACKSPACE, AC_KEY_INSERT, AC_KEY_DELETE, AC_KEY_RIGHT, AC_KEY_LEFT, AC_KEY_DOWN, AC_KEY_UP, AC_KEY_PAGE_UP, AC_KEY_PAGE_DOWN, AC_KEY_HOME, AC_KEY_END, AC_KEY_CAPS_LOCK, AC_KEY_SCROLL_LOCK, AC_KEY_NUM_LOCK, AC_KEY_PRINT_SCREEN, AC_KEY_PAUSE,
    AC_KEY_F1, AC_KEY_F2, AC_KEY_F3, AC_KEY_F4, AC_KEY_F5, AC_KEY_F6, AC_KEY_F7, AC_KEY_F8, AC_KEY_F9, AC_KEY_F10, AC_KEY_F11, AC_KEY_F12, AC_KEY_F13, AC_KEY_F14, AC_KEY_F15, AC_KEY_F16, AC_KEY_F17, AC_KEY_F18, AC_KEY_F19, AC_KEY_F20, AC_KEY_F21, AC_KEY_F22, AC_KEY_F23, AC_KEY_F24, AC_KEY_F25,
    AC_KEY_KP_0, AC_KEY_KP_1, AC_KEY_KP_2, AC_KEY_KP_3, AC_KEY_KP_4, AC_KEY_KP_5, AC_KEY_KP_6, AC_KEY_KP_7, AC_KEY_KP_8, AC_KEY_KP_9, AC_KEY_KP_DECIMAL, AC_KEY_KP_DIVIDE, AC_KEY_KP_MULTIPLY, AC_KEY_KP_SUBTRACT, AC_KEY_KP_ADD, AC_KEY_KP_ENTER, AC_KEY_KP_EQUAL,
    AC_KEY_LEFT_SHIFT, AC_KEY_LEFT_CONTROL, AC_KEY_LEFT_ALT, AC_KEY_LEFT_SUPER, AC_KEY_RIGHT_SHIFT, AC_KEY_RIGHT_CONTROL, AC_KEY_RIGHT_ALT, AC_KEY_RIGHT_SUPER, AC_KEY_MENU
};