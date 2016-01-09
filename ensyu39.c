#include "user.h"

static void MoveEnemy(void);
static void MoveBullet(void);
static void MoveFort(void);
static void UpdateLED(void);

volatile uchar sw = 0;
volatile uchar led[LED_SZ];

static uchar enemy[LED_SZ];
static uchar bullet[LED_SZ];
static uchar fort = 3;

void user_init(void)
{
	uchar i;
	for(i=0;i<LED_SZ;i++)
	{
		enemy[i] = 0;
		bullet[i] =0;
	}
}

void user_main(void)
{
	MoveEnemy();
	MoveBullet();
	MoveFort();
	UpdateLED();
}

static void MoveEnemy(void)
{
	static uchar tick = 0;
	static uchar dir = 0;
	uchar i;
	
	if(++tick<10){
		return;
	}
	
	tick = 0;
	
	switch(dir){
	case 0:
		for(i=0;i<LED_SZ;i++)
			enemy[i] >>= 1;
		break;
		
	case 2:
		for(i=0;i<LED_SZ;i++)
			enemy[i] <<= 1;
		break;
		
	default:
		for(i=0;i<(LED_SZ - 1);i++)
			enemy[i] = enemy[i+1];
		enemy[LED_SZ-1] = dir == 3 ? 0xaa : 0;
		break;
	}
	dir = (dir + 1) & 3;
}

static void MoveBullet(void)
{
	uchar i;
	for(i=LED_SZ-1;i;i--)
	bullet[i] = bullet[i-1];
	bullet[0] = 0;
}

static void MoveFort(void)
{
	static int i = 0;
	if(i>=1){
		i++;
		if(i>=8){
			i=0;
		}
	}
	
	switch(sw){
	case 1:
		if(fort > 1)
			fort--;
		break;
		
	case 2:
		if(fort < (LED_SZ - 2))
			fort++;
		break;
		
	case 3:
		if(i==0){
			bullet[i] |= 0x80 >> fort;
			i++;
		}
		break;
	}
}

static void UpdateLED(void)
{
	uchar i,j;
	
	for(i=0;i<LED_SZ;i++){
		j=enemy[i] & bullet[i];
		if(j){
			enemy[i] ^= j;
			bullet[i] ^= j;
		}
		led[i] = enemy[i] | bullet[i];
	}
	led[0] |= (uchar)(0x1c0 >> fort);
	led[1] |= (uchar)(0x80 >> fort);
}
