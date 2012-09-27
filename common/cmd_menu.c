/*
 * tq_668@126.com, www.embedsky.net
 *
 */

#include <common.h>
#include <command.h>
#include <def.h>
#include <nand.h>

#ifdef CONFIG_SURPORT_WINCE
#include "../wince/loader.h"
#endif

extern char console_buffer[];
extern int readline (const char *const prompt);
extern char awaitkey(unsigned long delay, int* error_p);
extern void download_nkbin_to_flash(void);
extern int boot_zImage(ulong from, size_t size);
extern char bLARGEBLOCK;

/**
 * Parses a string into a number.  The number stored at ptr is
 * potentially suffixed with K (for kilobytes, or 1024 bytes),
 * M (for megabytes, or 1048576 bytes), or G (for gigabytes, or
 * 1073741824).  If the number is suffixed with K, M, or G, then
 * the return value is the number multiplied by one kilobyte, one
 * megabyte, or one gigabyte, respectively.
 *
 * @param ptr where parse begins
 * @param retptr output pointer to next char after parse completes (output)
 * @return resulting unsigned int
 */
static unsigned long memsize_parse2 (const char *const ptr, const char **retptr)
{
	unsigned long ret = simple_strtoul(ptr, (char **)retptr, 0);
	int sixteen = 1;

	switch (**retptr) {
		case 'G':
		case 'g':
			ret <<= 10;
		case 'M':
		case 'm':
			ret <<= 10;
		case 'K':
		case 'k':
			ret <<= 10;
			(*retptr)++;
			sixteen = 0;
		default:
			break;
	}

	if (sixteen)
		return simple_strtoul(ptr, NULL, 16);
	
	return ret;
}


void param_menu_usage()
{
	printf("\r\n##### Parameter Menu #####\r\n");
	printf("[1] Set NFS boot parameter \r\n");
	printf("[2] Set Yaffs boot parameter \r\n");
	printf("[3] Set parameter \r\n");
	printf("[4] View the parameters\r\n");
	printf("[d] Delete parameter \r\n");
	printf("[s] Save the parameters to Nand Flash \r\n");
	printf("[q] Return main Menu \r\n");
	printf("Enter your selection: ");
}


void param_menu_shell(void)
{
	char c;
	char cmd_buf[256];
	char name_buf[20];
	char val_buf[256];
	char param_buf1[25];
	char param_buf2[25];
	char param_buf3[25];
	char param_buf4[64];

	while (1)
	{
		param_menu_usage();
		c = awaitkey(-1, NULL);
		printf("%c\n", c);
		switch (c)
		{
			case '1':
			{
				sprintf(cmd_buf, "setenv bootargs ");

				printf("Enter the PC IP address:(xxx.xxx.xxx.xxx)\n");
				readline(NULL);
				strcpy(param_buf1,console_buffer);
				printf("Enter the S3C2440/EM2440 IP address:(xxx.xxx.xxx.xxx)\n");
				readline(NULL);
				strcpy(param_buf2,console_buffer);
				printf("Enter the Mask IP address:(xxx.xxx.xxx.xxx)\n");
				readline(NULL);
				strcpy(param_buf3,console_buffer);
				printf("Enter NFS directory:(eg: /opt/EmbedSky/root_nfs)\n");
				readline(NULL);
				strcpy(param_buf4,console_buffer);
				sprintf(cmd_buf, "setenv bootargs console=ttySAC0 root=/dev/nfs nfsroot=%s:%s ip=%s:%s:%s:%s:SKY2440.embedsky.net:eth0:off", param_buf1, param_buf4, param_buf2, param_buf1, param_buf2, param_buf3);
				printf("bootargs: console=ttySAC0 root=/dev/nfs nfsroot=%s:%s ip=%s:%s:%s:%s:SKY2440.embedsky.net:eth0:off\n",param_buf1, param_buf4, param_buf2, param_buf1, param_buf2, param_buf3);

				run_command(cmd_buf, 0);
		break;
			}

			case '2':
			{
			#if CONFIG_128MB_SDRAM
				sprintf(cmd_buf, "setenv bootargs noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0 mem=128M");
				printf("bootargs: noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0 mem=128M\n");
			#else
				sprintf(cmd_buf, "setenv bootargs noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0 panic=5");
				printf("bootargs: noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0 panic=5\n");
			#endif

				run_command(cmd_buf, 0);
				break;
			}

			case '3':
			{
				sprintf(cmd_buf, "setenv ");

				printf("Name: ");
				readline(NULL);
				strcat(cmd_buf, console_buffer);

				printf("Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '4':
			{
				strcpy(cmd_buf, "printenv ");
				printf("Name(enter to view all paramters): ");
				readline(NULL);
				strcat(cmd_buf, console_buffer);
				run_command(cmd_buf, 0);
				break;
			}

			case 'd':
			{
				sprintf(cmd_buf, "setenv ");

				printf("Name: ");
				readline(NULL);
				strcat(cmd_buf, console_buffer);

				run_command(cmd_buf, 0);
				break;
			}

			case 's':
			{
				sprintf(cmd_buf, "saveenv");
				run_command(cmd_buf, 0);
				break;
			}

			case 'q':
			{
				//sprintf(cmd_buf, "saveenv");
				//run_command(cmd_buf, 0);
				return;
				break;
			}
		}
	}
}


void erase_menu_usage()
{
	printf("\r\n##### Erase Nand Menu #####\r\n");
	printf("[1] Nand scrub - really clean NAND erasing bad blocks (UNSAFE) \r\n");
	printf("[2] Nand earse - clean NAND eraseing \r\n");
	printf("[q] Return main Menu \r\n");
	printf("Enter your selection: ");
}


void erase_menu_shell(void)
{
	char c;
	char cmd_buf[256];
	char *p = NULL;
	unsigned long size;
	unsigned long offset;
	struct mtd_info *mtd = &nand_info[nand_curr_device];

	while (1)
	{
		erase_menu_usage();
		c = awaitkey(-1, NULL);
		printf("%c\n", c);
		switch (c)
		{
			case '1':
			{
				strcpy(cmd_buf, "nand scrub ");
				run_command(cmd_buf, 0);
				break;
			}

			case '2':
			{
				strcpy(cmd_buf, "nand erase ");

				printf("Start address: ");
				readline(NULL);
				strcat(cmd_buf, console_buffer);

				printf("Size(eg. 4000000, 0x4000000, 64m and so on): ");
				readline(NULL);
				p = console_buffer;
				size = memsize_parse2(p, &p);
				sprintf(console_buffer, " %x", size);
				strcat(cmd_buf, console_buffer);

				run_command(cmd_buf, 0);
				break;
			}

			case 'q':
			{
				return;
				break;
			}
		}
	}
}

void lcd_menu_usage()
{
	printf("\r\n##### LCD Parameters Menu #####\r\n");
#if USER_SET
	printf("[1] LCD TYPE   - Set LCD TYPE \r\n");
	printf("[2] VBPD       - Set VBPD \r\n");
	printf("[3] VFPD       - Set VFPD \r\n");
	printf("[4] VSPW       - Set VSPW \r\n");
	printf("[5] HBPD       - Set HBPD \r\n");
	printf("[6] HFPD       - Set HFPD \r\n");
	printf("[7] HSPW       - Set HSPW \r\n");
	printf("[8] CLKVAL     - Set CLKVAL \r\n");
#else
	printf("[1] VBPD       - Set VBPD \r\n");
	printf("[2] VFPD       - Set VFPD \r\n");
	printf("[3] VSPW       - Set VSPW \r\n");
	printf("[4] HBPD       - Set HBPD \r\n");
	printf("[5] HFPD       - Set HFPD \r\n");
	printf("[6] HSPW       - Set HSPW \r\n");
	printf("[7] CLKVAL     - Set CLKVAL \r\n");
#endif
	printf("[s] Save the parameters to Nand Flash \r\n");
	printf("[q] Return main Menu \r\n");
	printf("Enter your selection: ");
}


void lcd_menu_shell(void)
{
	char c;
	char cmd_buf[256];
	char *p = NULL;
	unsigned long size;
	unsigned long offset;
	struct mtd_info *mtd = &nand_info[nand_curr_device];

	while (1)
	{
		lcd_menu_usage();
		c = awaitkey(-1, NULL);
		printf("%c\n", c);
		switch (c)
		{
#if USER_SET
			case '1':
			{
				sprintf(cmd_buf, "setenv dwLCD_TYPE");

				printf("1: 800*600\t\t    2: 800*480\n3: 640*480\t\t    4: 480*272\n5: 240*320\t\t    6: 320*240\n"); 
				printf("Please enter LCD TYPE Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '2':
			{
				sprintf(cmd_buf, "setenv dwVBPD");

				printf("Please enter VBPD Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '3':
			{
				sprintf(cmd_buf, "setenv dwVFPD");

				printf("Please enter VFPD Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '4':
			{
				sprintf(cmd_buf, "setenv dwVSPW");

				printf("Please enter VSPW Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '5':
			{
				sprintf(cmd_buf, "setenv dwHBPD");

				printf("Please enter HBPD Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '6':
			{
				sprintf(cmd_buf, "setenv dwHFPD");

				printf("Please enter HFPD Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '7':
			{
				sprintf(cmd_buf, "setenv dwHSPW");

				printf("Please enter HSPW Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '8':
			{
				sprintf(cmd_buf, "setenv dwCLKVAL");

				printf("Please enter CLKVAL Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

#else
			case '1':
			{
				sprintf(cmd_buf, "setenv dwVBPD");

				printf("Please enter VBPD Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '2':
			{
				sprintf(cmd_buf, "setenv dwVFPD");

				printf("Please enter VFPD Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '3':
			{
				sprintf(cmd_buf, "setenv dwVSPW");

				printf("Please enter VSPW Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '4':
			{
				sprintf(cmd_buf, "setenv dwHBPD");

				printf("Please enter HBPD Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '5':
			{
				sprintf(cmd_buf, "setenv dwHFPD");

				printf("Please enter HFPD Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '6':
			{
				sprintf(cmd_buf, "setenv dwHSPW");

				printf("Please enter HSPW Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

			case '7':
			{
				sprintf(cmd_buf, "setenv dwCLKVAL");

				printf("Please enter CLKVAL Value: ");
				readline(NULL);
				strcat(cmd_buf, " ");
				strcat(cmd_buf, console_buffer);
				printf("%s\n",cmd_buf);

				run_command(cmd_buf, 0);
				break;
			}

#endif
			case 's':
			{
				sprintf(cmd_buf, "saveenv");
				run_command(cmd_buf, 0);
				break;
			}

			case 'q':
			{
				//sprintf(cmd_buf, "saveenv");
				//run_command(cmd_buf, 0);
				return;
				break;
			}
		}
	}
}


void main_menu_usage(void)
{

	if (bBootFrmNORFlash())
		printf("\r\n#####	 Boot for Nor Flash Main Menu	#####\r\n");
	else
		printf("\r\n#####	 Boot for Nand Flash Main Menu	#####\r\n");
//	printf("\r\n####   LCD_TYPE is %s	   ####\r\n",CONFIG_EMBEDSKY_LCD_TYPE);


	printf("[1] Download u-boot or STEPLDR.nb1 or other bootloader to Nand Flash\r\n");
#ifdef CONFIG_SURPORT_WINCE  	
	printf("[2] Download Eboot to Nand Flash\r\n");
#else							//HJ_add start 20090709
	printf("[2] Download Eboot to Nand Flash\r\n");	//HJ_add end 20090709
#endif
	printf("[3] Download Linux Kernel to Nand Flash\r\n");
#ifdef CONFIG_SURPORT_WINCE	
	printf("[4] Download WinCE NK.bin to Nand Flash\r\n");
#endif
	printf("[5] Download CRAMFS image to Nand Flash\r\n");
	printf("[6] Download YAFFS image to Nand Flash\r\n");
	printf("[7] Download Program (uCOS-II or EM2440_Test) to SDRAM and Run it\r\n");
	printf("[8] Boot the system\r\n");
	printf("[9] Format the Nand Flash\r\n");
	printf("[0] Set the boot parameters\r\n");
	printf("[a] Download User Program (eg: uCOS-II or EM2440_Test)\r\n");
//	printf("[b] Download LOGO Picture (.bmp) to Nand  Flash \r\n");	//HJ_del 20090709
	printf("[b] Download LOGO Picture (.bin) to Nand  Flash \r\n");	//HJ_add 20090709
	printf("[l] Set LCD Parameters \r\n");

	if (bBootFrmNORFlash())
		printf("[o] Download u-boot to Nor Flash\r\n");
	printf("[r] Reboot u-boot\r\n");
	printf("[q] quit from menu\r\n");
	printf("Enter your selection: ");
}


void menu_shell(void)
{
	char c;
	char cmd_buf[200];
	char *p = NULL;
	unsigned long size;
	unsigned long offset;
	struct mtd_info *mtd = &nand_info[nand_curr_device];

	while (1)
	{
		main_menu_usage();
		c = awaitkey(-1, NULL);
		printf("%c\n", c);
		switch (c)
		{
			case '1':
			{
				strcpy(cmd_buf, "usbslave 1 0x30000000; nand erase bios; nand write.jffs2 0x30000000 bios $(filesize)");
				run_command(cmd_buf, 0);
				break;
			}
			
#ifdef CONFIG_SURPORT_WINCE            
			case '2':
			{
				offset = EBOOT_BLOCK * mtd->erasesize;
				size   = EBOOT_BLOCK_SIZE * mtd->erasesize;
				sprintf(cmd_buf, "usbslave 1 0x30000000; nand erase 0x%x 0x%x; nand write.jffs2 0x30000000 0x%x $(filesize)", offset, size, offset);
				run_command(cmd_buf, 0);
				break;
			}
#else					//HJ_add start 20090709
			case '2':
			{
				sprintf(cmd_buf, "usbslave 1 0x30000000; nand erase eboot; nand write.jffs2 0x30000000 eboot $(filesize)");
				run_command(cmd_buf, 0);
				break;
			}		//HJ_add end 20090709
#endif
			case '3':
			{
				strcpy(cmd_buf, "usbslave 1 0x30000000; nand erase kernel; nand write.jffs2 0x30000000 kernel $(filesize)");
				run_command(cmd_buf, 0);
#ifdef CONFIG_SURPORT_WINCE
				if (!TOC_Read())
					TOC_Erase();				
#endif				
				break;
			}

#ifdef CONFIG_SURPORT_WINCE  
			case '4':
			{
				download_nkbin_to_flash();
				break;
			}
#endif
			case '5':
			{
				strcpy(cmd_buf, "usbslave 1 0x30000000; nand erase root; nand write.jffs2 0x30000000 root $(filesize)");
				run_command(cmd_buf, 0);
				break;
			}

			case '6':
			{
				strcpy(cmd_buf, "usbslave 1 0x30000000; nand erase root; nand write.yaffs 0x30000000 root $(filesize)");
				run_command(cmd_buf, 0);
				break;
			}

			case '7':
			{
				extern volatile U32 downloadAddress;
				extern int download_run;
				
				download_run = 1;
				strcpy(cmd_buf, "usbslave 1");
				run_command(cmd_buf, 0);
				download_run = 0;
				sprintf(cmd_buf, "go %x", downloadAddress);
				run_command(cmd_buf, 0);
				break;
			}

			case '8':
			{
			#ifdef CONFIG_EMBEDSKY_LOGO
				embedsky_user_logo();					//user's logo display
			#endif

#ifdef CONFIG_SURPORT_WINCE
				if (!TOC_Read())
				{
					/* Launch wince */
					printf("Start WinCE ...\n");
					strcpy(cmd_buf, "wince");
					run_command(cmd_buf, 0);
				}
				else
#endif
				{
					printf("Start Linux ...\n");
					strcpy(cmd_buf, "boot_zImage");
					run_command(cmd_buf, 0);
				}
				break;
			}

			case '9':
			{
				erase_menu_shell();
				break;
			}

			case '0':
			{
				param_menu_shell();
				break;
			}

			case 'a':
			{
				strcpy(cmd_buf, "usbslave 1 0x30000000; nand erase 0x0 $(filesize+1); nand write.jffs2 0x30000000 0x0 $(filesize+1)");
				run_command(cmd_buf, 0);
				break;
			}
/*	//HJ_del start 20090709
			case 'b':
			{
				strcpy(cmd_buf, "usbslave 1 0x30000000; nand erase logo; nand write.logo 0x30000000 logo $(filesize)");
				run_command(cmd_buf, 0);
				break;
			}
*/	//HJ_del start 20090709
	//HJ_add start 20090709
			case 'b':
			{
				strcpy(cmd_buf, "usbslave 1 0x30000000; nand erase logo; nand write.jffs2 0x30000000 logo $(filesize)");
				run_command(cmd_buf, 0);
				break;
			}
	//HJ_add end 20090709
			case 'l':
			{
				lcd_menu_shell();
				break;
			}

			case 'o':
			{
				if (bBootFrmNORFlash())
				{
					strcpy(cmd_buf, "usbslave 1 0x30000000; protect off all; erase 0 +$(filesize); cp.b 0x30000000 0 $(filesize)");
					run_command(cmd_buf, 0);
				}
				break;
			}

			case 'r':
			{
				strcpy(cmd_buf, "reset");
				run_command(cmd_buf, 0);
				break;
			}
			
			case 'q':
			{
				return;	
				break;
			}

		}
				
	}
}

int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	menu_shell();
	return 0;
}

U_BOOT_CMD(
	menu,	3,	0,	do_menu,
	"menu - display a menu, to select the items to do something\n",
	" - display a menu, to select the items to do something"
);

