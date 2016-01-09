/*
user.h : �A�v���P�[�V�����E�t���[�����[�N

Copyright(C) 2011-2015 Osamu Tamura @ Recursion Co., Ltd.
All rights reserved.
*/
#define LED_SZ 8

typedef unsigned char uchar;

 /* �u�U�[�p*/
 enum
 {
 BEEP_HIGH = 46,
 BEEP_LOW = 168,

 BEEP_C4 = 238,
 BEEP_D4 = 212,
 BEEP_E4 = 189,
 BEEP_F4 = 178,
 BEEP_G4 = 158,
 BEEP_A4 = 141,
 BEEP_B4 = 126,
 BEEP_C5 = 118
 };

 /* �V�X�e����`*/
 extern void _wait(uchar wait); /* ���ԑ҂�*/
 extern uchar _rand(void); /* �[����������*/
 extern void _sound(uchar tone, uchar length); /* �u�U�[*/

 extern volatile uchar sw; /* �����{�^�����*/
 extern volatile uchar led[LED_SZ]; /* �}�g���N�XLED */

 /* ���[�U�[��`*/
 extern void user_init(void);
 extern void user_main(void);