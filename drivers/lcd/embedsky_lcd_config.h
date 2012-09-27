#ifdef CONFIG_EMBEDSKY_LOGO
/*
#define TFT800600		1
#define TFT800480		2
#define VGA			3					//使用VGA时,需要修改board/SKY2440/config.mk文件,把值改为0x33F2A000
#define TFT480272		4
#define TFT240320		5
#define W35			6

#define LCD_TFT			W35
*/
#define MVAL			(13)
#define MVAL_USED 		(0)						//0=each frame   1=rate by MVAL
#define INVVDEN			(1)						//0=normal       1=inverted
#define BSWP			(0)						//Byte swap control
#define HWSWP			(1)						//Half word swap control


#if ( LCD_TFT == TFT800480 )
//TFT 800480
#define LCD_XSIZE_TFT	 	(800)	
#define LCD_YSIZE_TFT	 	(480)

//Timing parameter for LCD LTP700WV-F01

#define VBPD			(4)						//垂直同步信号的后肩
#define VFPD			(2)						//垂直同步信号的前肩
#define VSPW			(2)						//垂直同步信号的脉宽

#define HBPD			(27)						//水平同步信号的后肩
#define HFPD			(14)						//水平同步信号的前肩
#define HSPW			(8)						//水平同步信号的脉宽

//Timing parameter for LCD AT070TN83						//兰花电子使用的7寸屏
/*
#define VBPD			(21)						//垂直同步信号的后肩
#define VFPD			(5)						//垂直同步信号的前肩
#define VSPW			(2)						//垂直同步信号的脉宽

#define HBPD			(1)						//水平同步信号的后肩
#define HFPD			(1)						//水平同步信号的前肩
#define HSPW			(47)						//水平同步信号的脉宽
*/
#define CLKVAL_TFT		(2)

#elif ( LCD_TFT == VGA )
//VGA
#define LCD_XSIZE_TFT	 	(640)	
#define LCD_YSIZE_TFT	 	(480)

//Timing parameter for VGA
#define VBPD			(29)						//垂直同步信号的后肩
#define VFPD			(5)						//垂直同步信号的前肩
#define VSPW			(4)						//垂直同步信号的脉宽

#define HBPD			(71)						//水平同步信号的后肩
#define HFPD			(40)						//水平同步信号的前肩
#define HSPW			(31)						//水平同步信号的脉宽

#define CLKVAL_TFT		(1)

#elif ( LCD_TFT == TFT480272 )
//TFT 480272
#define LCD_XSIZE_TFT	 	(480)	
#define LCD_YSIZE_TFT	 	(272)

//Timing parameter for LCD LTV350QV-F0E
#define VBPD			(3)						//垂直同步信号的后肩
#define VFPD			(5)						//垂直同步信号的前肩
#define VSPW			(15)						//垂直同步信号的脉宽

#define HBPD			(5)						//水平同步信号的后肩
#define HFPD			(15)						//水平同步信号的前肩
#define HSPW			(8)						//水平同步信号的脉宽

#define CLKVAL_TFT		(7)

#elif ( LCD_TFT == TFT240320 )
//TFT 240320
#define LCD_XSIZE_TFT	 	(240)	
#define LCD_YSIZE_TFT	 	(320)

//Timing parameter for LCD 东芝屏
#define VBPD			(1)						//垂直同步信号的后肩
#define VFPD			(2)						//垂直同步信号的前肩
#define VSPW			(1)						//垂直同步信号的脉宽

#define HBPD			(6)						//水平同步信号的后肩
#define HFPD			(3)						//水平同步信号的前肩
#define HSPW			(1)						//水平同步信号的脉宽

#define CLKVAL_TFT		(4)

#elif ( LCD_TFT == W35 )
//TFT 320240
#define LCD_XSIZE_TFT	 	(320)	
#define LCD_YSIZE_TFT	 	(240)

//Timing parameter for LCD WXCAT35-TG3
#define VBPD			(12)						//垂直同步信号的后肩
#define VFPD			(4)						//垂直同步信号的前肩
#define VSPW			(5)						//垂直同步信号的脉宽

#define HBPD			(21)						//水平同步信号的后肩
#define HFPD			(32)						//水平同步信号的前肩
#define HSPW			(44)						//水平同步信号的脉宽

#define CLKVAL_TFT		(6)

#elif ( LCD_TFT == TFT800600 )
//TFT 800600
#define LCD_XSIZE_TFT	 	(800)	
#define LCD_YSIZE_TFT	 	(600)

//Timing parameter for LCD WXCAT35-TG3
#define VBPD			(22)						//垂直同步信号的后肩
#define VFPD			(36)						//垂直同步信号的前肩
#define VSPW			(5)						//垂直同步信号的脉宽

#define HBPD			(63)						//水平同步信号的后肩
#define HFPD			(55)						//水平同步信号的前肩
#define HSPW			(119)						//水平同步信号的脉宽

#define CLKVAL_TFT		(0)

#endif

#define USER_SET			0

#endif
