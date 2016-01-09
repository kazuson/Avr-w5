
/*
main.c : �A�v���P�[�V�����E�t���[�����[�N

Copyright(C) 2011-2015 Osamu Tamura @ Recursion Co.,
d.
All rights reserved.
*/

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <avr/wdt.h>

 #include "user.h"


 volatile uchar sw; /* �����{�^�����*/
 volatile uchar led[LED_SZ]; /* �}�g���N�XLED */

 static volatile uchar scan; /* LED �����J�E���^
*/
 static volatile uchar clk; /* �Ԉ����p�N���b�N*/
 static volatile uchar period; /* �u�U�[���̒���
*/
  static volatile uchar user; /* ���[�U�[����*/
  static volatile uchar delay; /* �҂����ԃJ�E���^
*/
  static volatile uchar swnow; /* �����{�^�����ݒl
*/
  static volatile uchar swnew; /* �����{�^���X�V�l
*/
  static volatile uchar swcnt; /* �����{�^��������
*/
  static volatile uchar rnd; /* �[�������̃J�E���^
*/

 /* �^�C�}���荞��*/
 ISR(TIMER0_COMPA_vect)
 {
 unsigned char sc;

 /* 2 mS ����LED ����*/
 PORTB = 0;
 scan = (scan + 1) & 7;
 sc = ~(1 << scan);
 PORTC = 0x30 | (sc & 0x0f); /* sw1,2 ���v���A
�v*/
 PORTD = sc & 0xf0;
 PORTB = led[scan];

 /* �X�C�b�`�ǂ�*/
 sc = PINC & 0x30;
 if (sc != swnow) {
 /* �`���^�����O�h�~*/
 if (sc == swnew) {
 if (++swcnt > 20) {
 swnow = swnew;
 sw = (swnow >> 4) ^ 3;
 }
 }
 else {
 swnew = sc;
 swcnt = 0;
 }
 // �����X�V
 rnd = rnd + scan + sw;
 }

 /* 100mS ���ɋN��*/
 if (++clk >= 50) {
 clk = 0;
 if (delay)
 delay--;

 /* �u�U�[��~*/
 if (period) {
 if (--period == 0)
 TCCR2A = 0;
 }
 user = 1;

 }
 }


 int main(void)
 {
 /* LED�A�X�C�b�`�A�u�U�[�̃|�[�g�ݒ�*/
 DDRB = 0xff;
 DDRC = 0x0f;
 DDRD = 0xfa;
 PORTC = 0x30;

 swnow = 0x30;
 swnew = 0x30;
 sw = 0;

 /* �^�C�}���荞�ݐݒ�*/
 OCR0A = 249; /* 2mS */
 TCCR0A = 2;
 TCCR0B = 3; /* 1/64 */
 TIMSK0 |= (1 << OCIE0A);

 /* �u�U�[�p�^�C�}�ݒ�*/
 TCCR2A = 0;
 TCCR2B = 0x44; /* 1/64 */

 /* �Q�[��������*/
 user_init();

 sei();
 for (user = 0;;) {
 wdt_reset();

 /* ���[�U�[�����̋N��*/
 if (user) {
 user_main();
 user = 0;
 }
 }

 return 0;
 }


 /*
 ���ԑ҂�
 */
 void _wait(uchar wait)
 {
 for (delay = wait; delay;) {
 wdt_reset();
 }
 }

 /*
 �[����������
 */
 uchar _rand(void)
 {
 /* �����X�V*/
 rnd++;

 /* �O���C�R�[�h�J�E���^*/
 return (uchar)(rnd ^ (rnd << 1));
 }

 /*
 �u�U�[
 */
 void _sound(uchar tone, uchar length)
 {
 OCR2A = tone;
 period = length;
 TCCR2A = 0x12;
 }