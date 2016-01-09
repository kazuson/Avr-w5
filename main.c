
/*
main.c : アプリケーション・フレームワーク

Copyright(C) 2011-2015 Osamu Tamura @ Recursion Co.,
d.
All rights reserved.
*/

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <avr/wdt.h>

 #include "user.h"


 volatile uchar sw; /* 押しボタン状態*/
 volatile uchar led[LED_SZ]; /* マトリクスLED */

 static volatile uchar scan; /* LED 走査カウンタ
*/
 static volatile uchar clk; /* 間引き用クロック*/
 static volatile uchar period; /* ブザー音の長さ
*/
  static volatile uchar user; /* ユーザー処理*/
  static volatile uchar delay; /* 待ち時間カウンタ
*/
  static volatile uchar swnow; /* 押しボタン現在値
*/
  static volatile uchar swnew; /* 押しボタン更新値
*/
  static volatile uchar swcnt; /* 押しボタン持続回数
*/
  static volatile uchar rnd; /* 擬似乱数のカウンタ
*/

 /* タイマ割り込み*/
 ISR(TIMER0_COMPA_vect)
 {
 unsigned char sc;

 /* 2 mS 毎にLED 走査*/
 PORTB = 0;
 scan = (scan + 1) & 7;
 sc = ~(1 << scan);
 PORTC = 0x30 | (sc & 0x0f); /* sw1,2 をプルア
プ*/
 PORTD = sc & 0xf0;
 PORTB = led[scan];

 /* スイッチ読み*/
 sc = PINC & 0x30;
 if (sc != swnow) {
 /* チャタリング防止*/
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
 // 乱数更新
 rnd = rnd + scan + sw;
 }

 /* 100mS 毎に起動*/
 if (++clk >= 50) {
 clk = 0;
 if (delay)
 delay--;

 /* ブザー停止*/
 if (period) {
 if (--period == 0)
 TCCR2A = 0;
 }
 user = 1;

 }
 }


 int main(void)
 {
 /* LED、スイッチ、ブザーのポート設定*/
 DDRB = 0xff;
 DDRC = 0x0f;
 DDRD = 0xfa;
 PORTC = 0x30;

 swnow = 0x30;
 swnew = 0x30;
 sw = 0;

 /* タイマ割り込み設定*/
 OCR0A = 249; /* 2mS */
 TCCR0A = 2;
 TCCR0B = 3; /* 1/64 */
 TIMSK0 |= (1 << OCIE0A);

 /* ブザー用タイマ設定*/
 TCCR2A = 0;
 TCCR2B = 0x44; /* 1/64 */

 /* ゲーム初期化*/
 user_init();

 sei();
 for (user = 0;;) {
 wdt_reset();

 /* ユーザー処理の起動*/
 if (user) {
 user_main();
 user = 0;
 }
 }

 return 0;
 }


 /*
 時間待ち
 */
 void _wait(uchar wait)
 {
 for (delay = wait; delay;) {
 wdt_reset();
 }
 }

 /*
 擬似乱数生成
 */
 uchar _rand(void)
 {
 /* 乱数更新*/
 rnd++;

 /* グレイコードカウンタ*/
 return (uchar)(rnd ^ (rnd << 1));
 }

 /*
 ブザー
 */
 void _sound(uchar tone, uchar length)
 {
 OCR2A = tone;
 period = length;
 TCCR2A = 0x12;
 }