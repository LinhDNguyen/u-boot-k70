/*
 *
 *			LCD初始化&显示logo
 *			版权所有:天嵌科技
 *			支持3.5寸LCD,VGA,7寸屏
 *
 */

#include <common.h>
#include <s3c2410.h>
#include <s3c24x0.h>

#ifdef CONFIG_EMBEDSKY_LOGO

#include "embedsky_lcd_config.h"
#include "embedsky_lcd.h"
#include "../drivers/lcd/embedsky_lcd_1.h"

#define U32 unsigned int

#define M5D(n)			( ( n ) & 0x1fffff )				// To get lower 21bits
extern char bLARGEBLOCK;			//HJ_add 20090807
extern char b128MB;				//HJ_add 20090807
extern int NF_ReadID(void);				//HJ_add 20090807
//extern int nand_read_ll(unsigned char*, unsigned long, int);

#if USER_SET
	union 
	{
		volatile unsigned short embedsky_LCD_BUFFER1[600][1600] ;
		volatile unsigned short embedsky_LCD_BUFFER2[480][1600] ;
		volatile unsigned short embedsky_LCD_BUFFER3[480][1280] ;
		volatile unsigned short embedsky_LCD_BUFFER4[272][960] ;
		volatile unsigned short embedsky_LCD_BUFFER5[320][480] ;
		volatile unsigned short embedsky_LCD_BUFFER6[240][640] ;
	}embedsky_buffer;

	volatile int xsize_tft = 320, ysize_tft = 240;
	volatile char lcd_type_set=1 ;
#else
//	#define SCR_XSIZE_TFT	 	(LCD_XSIZE_TFT * 2)	//HJ_del 20090709
	#define SCR_XSIZE_TFT	 	(LCD_XSIZE_TFT)		//HJ_del 20090709
	#define SCR_YSIZE_TFT	 	(LCD_YSIZE_TFT)

	#define HOZVAL_TFT		( LCD_XSIZE_TFT - 1 )
	#define LINEVAL_TFT		( LCD_YSIZE_TFT - 1 )

	volatile unsigned short embedsky_LCD_BUFFER[SCR_YSIZE_TFT][SCR_XSIZE_TFT] = { 0, } ;
#endif
volatile char vbpd = 1, vfpd = 1, vspw = 1, hbpd = 1, hfpd = 1, hspw = 1, clkval_tft = 1 ;


/**************************************************************
TFT LCD全屏填充特定颜色单元或清屏
**************************************************************/
void embedsky_lcd_ClearScr( U32 c)
{
	unsigned int x,y ;

#if USER_SET
	for( y = 0 ; y < ysize_tft ; y ++)
	{
		for( x = 0 ; x < (xsize_tft * 2) ; x++ )
		{
			switch(lcd_type_set)
			{
				case 1:
					embedsky_buffer.embedsky_LCD_BUFFER1[y][x] = c;
					break;
				case 2:
					embedsky_buffer.embedsky_LCD_BUFFER2[y][x] = c;
					break;
				case 3:
					embedsky_buffer.embedsky_LCD_BUFFER3[y][x] = c;
					break;
				case 4:
					embedsky_buffer.embedsky_LCD_BUFFER4[y][x] = c;
					break;
				case 5:
					embedsky_buffer.embedsky_LCD_BUFFER5[y][x] = c;
					break;
				case 6:
					embedsky_buffer.embedsky_LCD_BUFFER6[y][x] = c;
					break;
				defualt:
					embedsky_buffer.embedsky_LCD_BUFFER6[y][x] = c;
					break;
			}
#else
	for( y = 0 ; y < LCD_YSIZE_TFT ; y++ )
	{
		for( x = 0 ; x < (SCR_XSIZE_TFT) ; x++ )
		{
			embedsky_LCD_BUFFER[y][x] = c;
#endif
		}
	}
}

/**************************************************************
TFT LCD指定大小的矩形填充特定颜色单元或清屏
**************************************************************/
void embedsky_lcd_ClearScr_Rectangle( int x0,int y0,int width,int height, U32 c)
{
	unsigned int x,y ;
		
	for( y = y0 ; y < (y0 + height) ; y++ )
	{
		for( x = x0 ; x < (width + x0) ; x++ )
		{
#if USER_SET
			switch(lcd_type_set)
			{
				case 1:
					embedsky_buffer.embedsky_LCD_BUFFER1[y][x] = c;
					break;
				case 2:
					embedsky_buffer.embedsky_LCD_BUFFER2[y][x] = c;
					break;
				case 3:
					embedsky_buffer.embedsky_LCD_BUFFER3[y][x] = c;
					break;
				case 4:
					embedsky_buffer.embedsky_LCD_BUFFER4[y][x] = c;
					break;
				case 5:
					embedsky_buffer.embedsky_LCD_BUFFER5[y][x] = c;
					break;
				case 6:
					embedsky_buffer.embedsky_LCD_BUFFER6[y][x] = c;
					break;
				defualt:
					embedsky_buffer.embedsky_LCD_BUFFER6[y][x] = c;
					break;
			}
#else
			embedsky_LCD_BUFFER[y][x] = c;
#endif
		}
	}
}

/**************************************************************
在LCD屏幕上指定坐标点画一个指定大小的图片
**************************************************************/
void embedsky_paint_Bmp(int x0,int y0,int width,int height, unsigned char bmp[])
{
	int x, y ;
	U32 c ;
	int p = 0 ;

	for( y = y0 ; y < (height + y0) ; y++ )
	{
		for( x = x0 ; x < (width + x0) ; x++ )
		{
			c = bmp[p+1] | (bmp[p] << 8) ;

#if USER_SET
			if ( ( x <= (xsize_tft * 2) ) && ( y <= ysize_tft ) )
			{
				switch(lcd_type_set)
				{
					case 1:
						embedsky_buffer.embedsky_LCD_BUFFER1[y][x] = c;
						break;
					case 2:
						embedsky_buffer.embedsky_LCD_BUFFER2[y][x] = c;
						break;
					case 3:
						embedsky_buffer.embedsky_LCD_BUFFER3[y][x] = c;
						break;
					case 4:
						embedsky_buffer.embedsky_LCD_BUFFER4[y][x] = c;
						break;
					case 5:
						embedsky_buffer.embedsky_LCD_BUFFER5[y][x] = c;
						break;
					case 6:
						embedsky_buffer.embedsky_LCD_BUFFER6[y][x] = c;
						break;
					defualt:
						embedsky_buffer.embedsky_LCD_BUFFER6[y][x] = c;
						break;
				}
			}
#else
			if ( ( x <= (SCR_XSIZE_TFT) ) && ( y <= LCD_YSIZE_TFT ) )
				embedsky_LCD_BUFFER[y][x] = c;
#endif
//printk("x=%d  y=%d\n",x,y);			
			p = p + 2 ;
		}
	}

}

/**************************************************************
在LCD上指定位置画点
**************************************************************/
void putPixel(U32 x, U32 y, U32 c)
{
#if USER_SET
	if( (x < xsize_tft) && (y < ysize_tft))
	{
		switch(lcd_type_set)
		{
			case 1:
				embedsky_buffer.embedsky_LCD_BUFFER1[y][x] = c;
				break;
			case 2:
				embedsky_buffer.embedsky_LCD_BUFFER2[y][x] = c;
				break;
			case 3:
				embedsky_buffer.embedsky_LCD_BUFFER3[y][x] = c;
				break;
			case 4:
				embedsky_buffer.embedsky_LCD_BUFFER4[y][x] = c;
				break;
			case 5:
				embedsky_buffer.embedsky_LCD_BUFFER5[y][x] = c;
				break;
			case 6:
				embedsky_buffer.embedsky_LCD_BUFFER6[y][x] = c;
				break;
			defualt:
				embedsky_buffer.embedsky_LCD_BUFFER6[y][x] = c;
				break;
		}
	}
#else
	if( (x < LCD_XSIZE_TFT) && (y < LCD_YSIZE_TFT))
		embedsky_LCD_BUFFER[y][x] = c;
#endif
}

/*******************************************************************
LCD滚动条的显示(在Nand Flash读取时调用)
current当前字节数，total总共的字节数，PBcolor滚动条的颜色
*******************************************************************/
U32 currWidth = 0;
void drawProcessBar(U32 total, U32 current )
{
	U32 const bar_height = 8;
#if USER_SET
	U32 bar_base = ysize_tft - bar_height;
	int i = (int) xsize_tft / 8;
#else
	U32 bar_base = LCD_YSIZE_TFT - bar_height;
	int i = (int) LCD_XSIZE_TFT / 8;
#endif
	U32 j;

	int pbcolor ;
//printk("tooal = %d, current = %d\n",total, current);
	if(total != -1)
	{
		j = 0;
#if USER_SET
		int bar_width = (int) xsize_tft * ((current * 1.0) / total);
#else
		int bar_width = (int) LCD_XSIZE_TFT * ((current * 1.0) / total);
#endif
		if (bar_width <= i)
			pbcolor = 0x7FF;					//sky blue
		else if((bar_width > i) && (bar_width <= i * 2))
			pbcolor = 0x1F;					//blue
		else if((bar_width > i * 2) && (bar_width <= i * 3))
			pbcolor = 0x0;					//black
		else if((bar_width > i * 3) && (bar_width <= i * 4))
			pbcolor = 0xF81F;					//purple
		else if((bar_width > i * 4) && (bar_width <= i * 5))
			pbcolor = 0xFFFF;					//white
		else if((bar_width > i * 5) && (bar_width <= i * 6))
			pbcolor = 0xF800;					//red
		else if((bar_width > i * 6) && (bar_width <= i * 7))
			pbcolor = 0xFFE0;					//yellow
		else if((bar_width > i * 7) && (bar_width <= i * 8))
			pbcolor = 0x7E0;					//green

		if(bar_width > currWidth)
		{
			for ( ; j < bar_height; j++)
			{
				putPixel(bar_width, j + bar_base, pbcolor);
			}
			currWidth = bar_width;
		}
//printk("currWidth= %d, width= %d, height= %d, bar_base= %d\n",currWidth, bar_width, j + bar_base, bar_base);
	}
}

/**************************************************************
TFT LCD 电源控制引脚使能
**************************************************************/
void embedsky_lcd_PowerEnable(int invpwren , int pwren)
{
	S3C24X0_GPIO * const gpio = S3C24X0_GetBase_GPIO() ;
	S3C24X0_LCD * const lcd = S3C24X0_GetBase_LCD() ;
										//GPG4 is setted as LCD_PWREN
	gpio -> GPGUP = gpio -> GPGUP & ( ~( 1 << 4) ) | ( 1 << 4) ;		// Pull-up disable
	gpio -> GPGCON = gpio -> GPGCON & ( ~( 3 << 8) ) | ( 3 << 8) ;		//GPG4=LCD_PWREN
	gpio -> GPGDAT = gpio -> GPGDAT | (1 << 4 ) ;
										//invpwren=pwren;
										//Enable LCD POWER ENABLE Function
	lcd -> LCDCON5 = lcd -> LCDCON5 & ( ~( 1 << 3 ) ) | ( pwren << 3 ) ;	// PWREN
	lcd -> LCDCON5 = lcd -> LCDCON5 & ( ~( 1 << 5 ) ) | ( invpwren << 5 ) ;	// INVPWREN
}

/**************************************************************
LCD视频和控制信号输出或者停止，1开启视频输出
**************************************************************/
void embedsky_lcd_EnvidOnOff(int onoff)
{
	S3C24X0_LCD * const lcd = S3C24X0_GetBase_LCD() ;

	if( onoff == 1 )
		lcd -> LCDCON1 |= 1 ;							// ENVID=ON
	else
		lcd -> LCDCON1 = lcd -> LCDCON1 & 0x3fffe ;				// ENVID Off
}

/**************************************************************
LCD初始化程序(在board/EmbedSky/目录下的boot_init.c文件的60行左右,调用的)
**************************************************************/
void embedsky_lcd_Init(void)
{
	int x, y ;
	U32 c ;
	int p = 0 ;

	S3C24X0_GPIO * const gpio = S3C24X0_GetBase_GPIO() ;
	S3C24X0_LCD * const lcd = S3C24X0_GetBase_LCD() ;

	char *s_lcd;

#if USER_SET
	s_lcd = getenv ("dwLCD_TYPE");
	lcd_type_set = s_lcd ? (int)simple_strtol(s_lcd, NULL, 10) : CONFIG_EMBEDSKY_LCD_TYPE;

	switch(lcd_type_set)
	{
		case 6:				//LCD_320*240
			xsize_tft = 320;
			ysize_tft = 240;
			lcd -> LCDSADDR1 = ( ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER6 >> 22 ) << 21 ) | M5D ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER6 >> 1 ) ;
			lcd -> LCDSADDR2 = M5D( ( ( U32) embedsky_buffer.embedsky_LCD_BUFFER6 + ( (xsize_tft*2) * ysize_tft * 2 ) ) >> 1 ) ;
			lcd -> LCDSADDR3 = ( ( ( (xsize_tft*2) - xsize_tft ) / 1 ) << 11 ) | ( xsize_tft /1 ) ;
			break;

		case 5:				//LCD_240*320
			xsize_tft = 240;
			ysize_tft = 320;
			lcd -> LCDSADDR1 = ( ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER5 >> 22 ) << 21 ) | M5D ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER5 >> 1 ) ;
			lcd -> LCDSADDR2 = M5D( ( ( U32) embedsky_buffer.embedsky_LCD_BUFFER5 + ( (xsize_tft*2) * ysize_tft * 2 ) ) >> 1 ) ;
			lcd -> LCDSADDR3 = ( ( ( (xsize_tft*2) - xsize_tft ) / 1 ) << 11 ) | ( xsize_tft /1 ) ;
			break;

		case 4:				//LCD_480*272
			xsize_tft = 480;
			ysize_tft = 272;
			lcd -> LCDSADDR1 = ( ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER4 >> 22 ) << 21 ) | M5D ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER4 >> 1 ) ;
			lcd -> LCDSADDR2 = M5D( ( ( U32) embedsky_buffer.embedsky_LCD_BUFFER4 + ( (xsize_tft*2) * ysize_tft * 2 ) ) >> 1 ) ;
			lcd -> LCDSADDR3 = ( ( ( (xsize_tft*2) - xsize_tft ) / 1 ) << 11 ) | ( xsize_tft /1 ) ;
			break;

		case 3:				//LCD_640*480
			xsize_tft = 640;
			ysize_tft = 480;
			lcd -> LCDSADDR1 = ( ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER3 >> 22 ) << 21 ) | M5D ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER3 >> 1 ) ;
			lcd -> LCDSADDR2 = M5D( ( ( U32) embedsky_buffer.embedsky_LCD_BUFFER3 + ( (xsize_tft*2) * ysize_tft * 2 ) ) >> 1 ) ;
			lcd -> LCDSADDR3 = ( ( ( (xsize_tft*2) - xsize_tft ) / 1 ) << 11 ) | ( xsize_tft /1 ) ;
			break;

		case 2:				//LCD_800*480
			xsize_tft = 800;
			ysize_tft = 480;
			lcd -> LCDSADDR1 = ( ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER2 >> 22 ) << 21 ) | M5D ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER2 >> 1 ) ;
			lcd -> LCDSADDR2 = M5D( ( ( U32) embedsky_buffer.embedsky_LCD_BUFFER2 + ( (xsize_tft*2) * ysize_tft * 2 ) ) >> 1 ) ;
			lcd -> LCDSADDR3 = ( ( ( (xsize_tft*2) - xsize_tft ) / 1 ) << 11 ) | ( xsize_tft /1 ) ;
			break;

		case 1:				//LCD_800*600
			xsize_tft = 800;
			ysize_tft = 600;
			lcd -> LCDSADDR1 = ( ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER1 >> 22 ) << 21 ) | M5D ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER1 >> 1 ) ;
			lcd -> LCDSADDR2 = M5D( ( ( U32) embedsky_buffer.embedsky_LCD_BUFFER1 + ( (xsize_tft*2) * ysize_tft * 2 ) ) >> 1 ) ;
			lcd -> LCDSADDR3 = ( ( ( (xsize_tft*2) - xsize_tft ) / 1 ) << 11 ) | ( xsize_tft /1 ) ;
			break;

		defualt:				//defualt is 320*240
			xsize_tft = 320;
			ysize_tft = 240;
			break;
			
	}
#else
	lcd -> LCDSADDR1 = ( ( ( U32 ) embedsky_LCD_BUFFER >> 22 ) << 21 ) | M5D ( ( U32 ) embedsky_LCD_BUFFER >> 1 ) ;
	lcd -> LCDSADDR2 = M5D( ( ( U32) embedsky_LCD_BUFFER + ( SCR_XSIZE_TFT * LCD_YSIZE_TFT * 2 ) ) >> 1 ) ;
	lcd -> LCDSADDR3 = ( ( ( SCR_XSIZE_TFT - LCD_XSIZE_TFT ) / 1 ) << 11 ) | ( LCD_XSIZE_TFT /1 ) ;
#endif
	s_lcd = getenv ("dwVBPD");
	vbpd = s_lcd ? (int)simple_strtol(s_lcd, NULL, 10) : CONFIG_EMBEDSKY_LCD_VBPD;

	s_lcd = getenv ("dwVFPD");
	vfpd = s_lcd ? (int)simple_strtol(s_lcd, NULL, 10) : CONFIG_EMBEDSKY_LCD_VFPD;

	s_lcd = getenv ("dwVSPW");
	vspw = s_lcd ? (int)simple_strtol(s_lcd, NULL, 10) : CONFIG_EMBEDSKY_LCD_VSPW;

	s_lcd = getenv ("dwHBPD");
	hbpd = s_lcd ? (int)simple_strtol(s_lcd, NULL, 10) : CONFIG_EMBEDSKY_LCD_HBPD;

	s_lcd = getenv ("dwHFPD");
	hfpd = s_lcd ? (int)simple_strtol(s_lcd, NULL, 10) : CONFIG_EMBEDSKY_LCD_HFPD;

	s_lcd = getenv ("dwHSPW");
	hspw = s_lcd ? (int)simple_strtol(s_lcd, NULL, 10) : CONFIG_EMBEDSKY_LCD_HSPW;

	s_lcd = getenv ("dwCLKVAL");
	clkval_tft = s_lcd ? (int)simple_strtol(s_lcd, NULL, 10) : CONFIG_EMBEDSKY_LCD_CLKVAL;

	embedsky_lcd_ClearScr( 0x0 ) ;

	gpio -> GPCUP  = 0xffffffff ;
	gpio -> GPCCON = 0xaaaaaaaa ;						//Initialize VD[0:7]	 
	 
	gpio -> GPDUP  = 0xffffffff ;
	gpio -> GPDCON = 0xaaaaaaaa ;						//Initialize VD[15:8]

	lcd -> LCDCON1 = ( clkval_tft << 8 ) | ( MVAL_USED << 7 ) | (3 << 5 ) | ( 12 << 1 ) | 0 ;
										// TFT LCD panel,16bpp TFT,ENVID=off
#if USER_SET
	lcd -> LCDCON2 = ( vbpd << 24 ) | ( (ysize_tft - 1) << 14 ) | ( vfpd << 6 ) | ( vspw ) ;
	lcd -> LCDCON3 = ( hbpd << 19 ) | ( (xsize_tft - 1) << 8 ) | ( hfpd ) ;
#else
	lcd -> LCDCON2 = ( vbpd << 24 ) | ( LINEVAL_TFT << 14 ) | ( vfpd << 6 ) | ( vspw ) ;
	lcd -> LCDCON3 = ( hbpd << 19 ) | ( HOZVAL_TFT << 8 ) | ( hfpd ) ;
#endif
	lcd -> LCDCON4 = ( MVAL << 8 ) | ( hspw ) ;
	lcd -> LCDCON5 = ( 1 << 11) | ( 1 << 10 ) | ( 1 << 9 ) | ( 1 << 8 ) | ( 0 << 7 ) | ( 0 << 6 ) | ( 0 << 3 ) | ( BSWP << 1 ) | ( HWSWP ) ;

//	lcd -> LCDSADDR1 = ( ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER1 >> 22 ) << 21 ) | M5D ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER1 >> 1 ) ;
//	lcd -> LCDSADDR2 = M5D( ( ( U32) embedsky_buffer.embedsky_LCD_BUFFER1 + ( (xsize_tft*2) * ysize_tft * 2 ) ) >> 1 ) ;
//	lcd -> LCDSADDR3 = ( ( ( (xsize_tft*2) - xsize_tft ) / 1 ) << 11 ) | ( xsize_tft /1 ) ;
/*
	lcd -> LCDSADDR1 = ( ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER1 >> 22 ) << 21 ) | M5D ( ( U32 ) embedsky_buffer.embedsky_LCD_BUFFER1 >> 1 ) ;
	lcd -> LCDSADDR2 = M5D( ( ( U32) embedsky_buffer.embedsky_LCD_BUFFER1 + ( SCR_XSIZE_TFT * LCD_YSIZE_TFT * 2 ) ) >> 1 ) ;
	lcd -> LCDSADDR3 = ( ( ( SCR_XSIZE_TFT - LCD_XSIZE_TFT ) / 1 ) << 11 ) | ( LCD_XSIZE_TFT /1 ) ;
*/
	lcd -> LCDINTMSK |= (3) ;						// MASK LCD Sub Interrupt
	lcd -> LPCSEL &= ( ~7 ) ;							// Disable LPC3480
	lcd -> TPAL = 0 ;							// Disable Temp Palette

//printk("LCD_TYPE=%d\n",lcd_type_set);
	embedsky_lcd_PowerEnable( 0, 1 ) ;
}

/****************************************************************
LCD显示天嵌科技下载logo程序(在common/目录下的main.c文件的277行左右调用的)
****************************************************************/
void embedsky_tq_logo(void)
{
	S3C24X0_GPIO * const gpio = S3C24X0_GetBase_GPIO() ;
	S3C24X0_LCD * const lcd = S3C24X0_GetBase_LCD() ;

#if USER_SET
	embedsky_paint_Bmp((xsize_tft-170) / 2, ysize_tft / 8, 170, 48, logo_tqkj) ;
	embedsky_paint_Bmp((xsize_tft-170) / 2, ysize_tft / 8 + 50, 170, 22, logo_net) ;
	embedsky_paint_Bmp((xsize_tft -180) / 2, ysize_tft / 8 + 120, 180, 20, down_mode);	//显示进入下载模式，中间
#else
	if(1)
	{
		embedsky_paint_Bmp((LCD_XSIZE_TFT-170) / 2, LCD_YSIZE_TFT / 8, 170, 48, logo_tqkj) ;
		embedsky_paint_Bmp((LCD_XSIZE_TFT-170) / 2, LCD_YSIZE_TFT / 8 + 50, 170, 22, logo_net) ;
		embedsky_paint_Bmp((LCD_XSIZE_TFT -180) / 2, LCD_YSIZE_TFT / 8 + 120, 180, 20, down_mode);	//显示进入下载模式，中间
	}
	else
		embedsky_paint_Bmp((LCD_XSIZE_TFT -180) / 2, (LCD_YSIZE_TFT -20) / 2, 180, 20, down_mode);		//显示进入下载模式，中间	
#endif

	embedsky_lcd_EnvidOnOff ( 1 ) ;
//printk("xsize=%d,  ysize=%d\n", xsize_tft, ysize_tft);
//printk("xsize=%d,  ysize=%d type=%d vbpd=%d vfpd=%d vspw=%d hbpd=%d hfpd=% hspw=% clk=%d\n",xsize_tft, ysize_tft, lcd_type_set,vbpd,vfpd,vspw,hbpd,hfpd,hspw,clkval_tft);

}

/****************************************************************
LCD显示用户开机logo程序(在common/目录下的main.c文件的277行左右调用的)
****************************************************************/
void embedsky_user_logo(void)
{
	S3C24X0_GPIO * const gpio = S3C24X0_GetBase_GPIO() ;
	S3C24X0_LCD * const lcd = S3C24X0_GetBase_LCD() ;

#if USER_SET
	size_t retlen = ysize_tft * (xsize_tft * 2) * (16 >> 3);
	switch(lcd_type_set)
	{
		case 1:
if (NF_ReadID() == 0x76)
			nand_read_ll(embedsky_buffer.embedsky_LCD_BUFFER1, 0x70000, retlen);
else
			nand_read_ll_lp(embedsky_buffer.embedsky_LCD_BUFFER1, 0xA0000, retlen);
			break;
		case 2:
if (NF_ReadID() == 0x76)
			nand_read_ll(embedsky_buffer.embedsky_LCD_BUFFER2, 0x70000, retlen);
else
			nand_read_ll_lp(embedsky_buffer.embedsky_LCD_BUFFER2, 0xA0000, retlen);
			break;
		case 3:
if (NF_ReadID() == 0x76)
			nand_read_ll(embedsky_buffer.embedsky_LCD_BUFFER3, 0x70000, retlen);
else
			nand_read_ll_lp(embedsky_buffer.embedsky_LCD_BUFFER3, 0xA0000, retlen);
			break;
		case 4:
if (NF_ReadID() == 0x76)
			nand_read_ll(embedsky_buffer.embedsky_LCD_BUFFER4, 0x70000, retlen);
else
			nand_read_ll_lp(embedsky_buffer.embedsky_LCD_BUFFER4, 0xA0000, retlen);
			break;
		case 5:
if (NF_ReadID() == 0x76)
			nand_read_ll(embedsky_buffer.embedsky_LCD_BUFFER5, 0x70000, retlen);
else
			nand_read_ll_lp(embedsky_buffer.embedsky_LCD_BUFFER5, 0xA0000, retlen);
			break;
		case 6:
if (NF_ReadID() == 0x76)
			nand_read_ll(embedsky_buffer.embedsky_LCD_BUFFER6, 0x70000, retlen);
else
			nand_read_ll_lp(embedsky_buffer.embedsky_LCD_BUFFER6, 0xA0000, retlen);
			break;
		defualt:
if (NF_ReadID() == 0x76)
			nand_read_ll(embedsky_buffer.embedsky_LCD_BUFFER6, 0x70000, retlen);
else
			nand_read_ll_lp(embedsky_buffer.embedsky_LCD_BUFFER6, 0xA0000, retlen);
			break;
	}
	embedsky_lcd_ClearScr_Rectangle(0, ysize_tft-8, xsize_tft, 8, 0xFFFF);
#else
	size_t retlen = SCR_YSIZE_TFT * SCR_XSIZE_TFT * (16 >> 3);

if (NF_ReadID() == 0x76)
{
	if(( retlen % 0x200) != 0)				//HJ_add 20090709
		retlen = (retlen / 0x200) * 0x200 + 0x200;	//HJ_add 20090709
	nand_read_ll(embedsky_LCD_BUFFER, 0x100000, retlen);
}
else
{
	if(( retlen % 0x800) != 0)				//HJ_add 20090709
		retlen = (retlen / 0x800) * 0x800 + 0x800;	//HJ_add 20090709
	nand_read_ll_lp(embedsky_LCD_BUFFER, 0x100000, retlen);
}
	embedsky_lcd_ClearScr_Rectangle(0, LCD_YSIZE_TFT-8, LCD_XSIZE_TFT, 8, 0xFFFF);
#endif

	embedsky_lcd_EnvidOnOff ( 1 ) ;
//printk("xsize=%d,  ysize=%d\n", xsize_tft, ysize_tft);
//printk("xsize=%d,  ysize=%d type=%d vbpd=%d vfpd=%d vspw=%d hbpd=%d hfpd=% hspw=% clk=%d\n",xsize_tft, ysize_tft, lcd_type_set,vbpd,vfpd,vspw,hbpd,hfpd,hspw,clkval_tft);

}

//----------------------------------- Nand Flash Read -------------------------------------------

#define NAND_SECTOR_SIZE    512
#define NAND_BLOCK_MASK     (NAND_SECTOR_SIZE - 1)

#define NAND_SECTOR_SIZE_LP		2048
#define NAND_BLOCK_MASK_LP		(NAND_SECTOR_SIZE_LP - 1)

/* 等待NAND Flash就绪 */
static void EmbedSky_wait_idle(void)
{
	int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFSTAT;

	while(!(*p & 1))
	for(i=0; i<10; i++);
}

/* 发出片选信号 */
static void EmbedSky_nand_select_chip(void)
{
	int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

	s3c2440nand->NFCONT &= ~(1<<1);
	for(i=0; i<10; i++);
}

/* 取消片选信号 */
static void EmbedSky_nand_deselect_chip(void)
{
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

	s3c2440nand->NFCONT |= (1<<1);
}

/* 发出命令 */
static void EmbedSky_write_cmd(int cmd)
{
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;

	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFCMD;
	*p = cmd;
}

/* 发出地址 */
static void EmbedSky_write_addr(unsigned int addr)
{
	int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;

	*p = addr & 0xff;
	for(i=0; i<10; i++);
	*p = (addr >> 9) & 0xff;
	for(i=0; i<10; i++);
	*p = (addr >> 17) & 0xff;
	for(i=0; i<10; i++);
	*p = (addr >> 25) & 0xff;
	for(i=0; i<10; i++);
}

/* 发出地址 */
static void EmbedSky_write_addr_lp(unsigned int addr)
{
	int i;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;
	int col, page;

	col = addr & NAND_BLOCK_MASK_LP;
	page = addr / NAND_SECTOR_SIZE_LP;
	
	*p = col & 0xff;			/* Column Address A0~A7 */
	for(i=0; i<10; i++);
	*p = (col >> 8) & 0x0f;		/* Column Address A8~A11 */
	for(i=0; i<10; i++);
	*p = page & 0xff;			/* Row Address A12~A19 */
	for(i=0; i<10; i++);
	*p = (page >> 8) & 0xff;	/* Row Address A20~A27 */
	for(i=0; i<10; i++);
#if (b128MB == 0)
	*p = (page >> 16) & 0x03;	/* Row Address A28~A29 */
	for(i=0; i<10; i++);
#endif
}

/* 读取数据 */
static unsigned char EmbedSky_read_data(void)
{
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFDATA;
	return *p;
}


/* 读函数 添加了LCD的滚动条 */
int nand_read_ll_lcd(unsigned char *buf, unsigned long start_addr, int size)
{
	int i, j;
	char dat;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;


#ifdef CONFIG_EMBEDSKY_LOGO
	int current = 0;				//HJ_add 滚动条，当前的字节数
	int total = 0;				//HJ_add 滚动条
#endif

	if ((start_addr & 511) || (size & 511))
	{
		return ;		/* 地址或长度不对齐 */
	}

	/* 选中芯片 */
	EmbedSky_nand_select_chip();

#ifdef CONFIG_EMBEDSKY_LOGO
	total = size;			//HJ_add 滚动条
#endif

	for(i=start_addr; i < (start_addr + size);)
	{
if(0){
/* Check Bad Block */
		/* 发出READ0命令 */
		EmbedSky_write_cmd(0x50);

		*p = 5;
		for(j=0; j<10; j++);
		*p = (i >> 9) & 0xff;
		for(j=0; j<10; j++);
		*p = (i >> 17) & 0xff;
		for(j=0; j<10; j++);
		*p = (i >> 25) & 0xff;
		for(j=0; j<10; j++);
		EmbedSky_wait_idle();

		dat = EmbedSky_read_data();
		EmbedSky_write_cmd(0);
		
		/* 取消片选信号 */
		EmbedSky_nand_deselect_chip();
		if(dat != 0xff)
		{
			i += 16384;		// 1 Block = 512*32= 16384
			printf("\nBad block at 0x%lx ,will be skipped\n",i);
		}
/* Read Page */
		/* 选中芯片 */
		EmbedSky_nand_select_chip();
}
		/* 发出READ0命令 */
		EmbedSky_write_cmd(0);

		/* Write Address */
		EmbedSky_write_addr(i);
		EmbedSky_wait_idle();

		for(j=0; j < 512; j++, i++)
		{
			*buf = EmbedSky_read_data();
			buf++;
		}

#ifdef CONFIG_EMBEDSKY_LOGO
		current += 512 ;					//HJ_add 滚动条
		drawProcessBar(total, current );			//HJ_add 滚动条
#endif
	}

	/* 取消片选信号 */
	EmbedSky_nand_deselect_chip();

	return 0;
}

/* 读函数 添加了LCD的滚动条 */
int nand_read_ll_lp_lcd(unsigned char *buf, unsigned long start_addr, int size)
{
	int i, j;
#ifdef CONFIG_EMBEDSKY_LOGO
	int current = 0;				//HJ_add 滚动条，当前的字节数
	int total = 0;				//HJ_add 滚动条
#endif

	char dat;
	S3C2440_NAND * s3c2440nand = (S3C2440_NAND *)0x4e000000;
	volatile unsigned char *p = (volatile unsigned char *)&s3c2440nand->NFADDR;

	if ((start_addr & NAND_BLOCK_MASK_LP) || (size & NAND_BLOCK_MASK_LP))
	{
		return ;    /* 地址或长度不对齐 */
	}

	/* 选中芯片 */
	EmbedSky_nand_select_chip();
#ifdef CONFIG_EMBEDSKY_LOGO
	total = size;			//HJ_add 滚动条
#endif

	for(i=start_addr; i < (start_addr + size);)
	{
/* Check Bad Block */
if(0){
		int col, page;

		col = i & NAND_BLOCK_MASK_LP;
		page = i / NAND_SECTOR_SIZE_LP;
		/* 发出READ0命令 */
		EmbedSky_write_cmd(0x00);

		*p = 5;
		for(j=0; j<10; j++);
		*p = 8;
		for(j=0; j<10; j++);
		*p = page & 0xff;		/* Row Address A12~A19 */
		for(j=0; j<10; j++);
		*p = (page >> 8) & 0xff;		/* Row Address A20~A27 */
		for(j=0; j<10; j++);
#if (b128MB== 0)
		*p = (page >> 16) & 0x03;		/* Row Address A28~A29 */
		for(j=0; j<10; j++);
#endif
		EmbedSky_write_cmd(0x30);
		EmbedSky_wait_idle();
		dat = EmbedSky_read_data();
		
		/* 取消片选信号 */
		EmbedSky_nand_deselect_chip();
		if(dat != 0xff)
		{
			i += 131072;		// 1 Block = 2048*64= 131072
			printf("\nBad block at 0x%lx ,will be skipped\n",i);
		}
/* Read Page */
		/* 选中芯片 */
		EmbedSky_nand_select_chip();
}
		/* 发出READ0命令 */
		EmbedSky_write_cmd(0);

		/* Write Address */
		EmbedSky_write_addr_lp(i);
		EmbedSky_write_cmd(0x30);
		EmbedSky_wait_idle();

		for(j=0; j < NAND_SECTOR_SIZE_LP; j++, i++)
		{
			*buf = EmbedSky_read_data();
			buf++;
		}
#ifdef CONFIG_EMBEDSKY_LOGO
		current += 2048 ;				//HJ_add 滚动条
		drawProcessBar(total, current );			//HJ_add 滚动条
#endif
	}

	/* 取消片选信号 */
	EmbedSky_nand_deselect_chip();

	return 0;
}


#endif

