/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <s3c2410.h>

DECLARE_GLOBAL_DATA_PTR;


/*
 * Miscellaneous platform dependent initialisations
 */
int board_init (void)
{
	S3C24X0_CLOCK_POWER * const clk_power = S3C24X0_GetBase_CLOCK_POWER();
	S3C24X0_GPIO * const gpio = S3C24X0_GetBase_GPIO();

	/* set up the I/O ports */
	gpio->GPACON = 0x007FFFFF;
	gpio->GPBCON = 0x00055555;
	gpio->GPBUP = 0x000007FF;
	gpio->GPCCON = 0xAAAAAAAA;
	gpio->GPCUP = 0x0000FFFF;
	gpio->GPDCON = 0xAAAAAAAA;
	gpio->GPDUP = 0x0000FFFF;
	gpio->GPECON = 0xAAAAAAAA;
	gpio->GPEUP = 0x0000FFFF;
	gpio->GPFCON = 0x000055AA;
	gpio->GPFUP = 0x000000FF;
	gpio->GPGCON = 0xFF94FFBA;
	gpio->GPGUP = 0x0000FFEF;
	gpio->GPGDAT = gpio->GPGDAT & (~(1<<4)) | (1<<4) ;
	gpio->GPHCON = 0x002AFAAA;
	gpio->GPHUP = 0x000007FF;
	gpio->GPJCON = 0x02aaaaaa;
	gpio->GPJUP = 0x00001fff;

//	S3C24X0_I2S * const i2s = S3C24X0_GetBase_I2S();	//HJ_add 屏蔽IIS,
//	i2s->IISCON = 0x00;					//HJ_add 屏蔽IIS,

	/* support both of S3C2410 and S3C2440, by www.embedsky.net */
	if ((gpio->GSTATUS1 == 0x32410000) || (gpio->GSTATUS1 == 0x32410002))
	{
		/* arch number of SMDK2410-Board */
		gd->bd->bi_arch_number = MACH_TYPE_SMDK2410;
	}
	else
	{
		/* arch number of SMDK2440-Board */
		gd->bd->bi_arch_number = MACH_TYPE_S3C2440;
	}

	/* adress of boot parameters */
	gd->bd->bi_boot_params = 0x30000100;

	icache_enable();
	dcache_enable();

	return 0;
}

int dram_init (void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

	return 0;
}
