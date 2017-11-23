/*
 * (C) Copyright 2008 Byron Bradley <byron.bbradley@gmail.com>
 *
 * Modified for DNS-323 from Kuro Pro
 * Modified for Z5 from DNS-323
 * (C) Copyright 2008 Veli-Matti Valtonen <maligor@gmail.com>
 * (C) 2017 Dirk Su <dirksu@gmail.com>
 *
 * Configuation settings for the DNS-323 board.
 * Configuation settings for the Z5 board.
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

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * SPL
 */

#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_TEXT_BASE		0xffff0000
#define CONFIG_SPL_MAX_SIZE			0x0000fff0
#define CONFIG_SPL_STACK			0x00020000
#define CONFIG_SPL_BSS_START_ADDR	0x00020000
#define CONFIG_SPL_BSS_MAX_SIZE		0x0001ffff
#define CONFIG_SYS_SPL_MALLOC_START	0x00040000
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x0001ffff
#define CONFIG_SYS_UBOOT_BASE		0xfff90000
#define CONFIG_SYS_UBOOT_START		0x00800000
#define CONFIG_SYS_TEXT_BASE 		0x00800000

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_FEROCEON					1
#define CONFIG_88F5182					1	

#include <asm/arch/orion5x.h>

#undef CONFIG_USE_IRQ					/* we don't need IRQ/FIQ stuff	*/

#define CONFIG_CMDLINE_TAG	 			1	/* enable passing of ATAGs    	*/
#define CONFIG_SETUP_MEMORY_TAGS 		1
#define CONFIG_INITRD_TAG	 			1

#define CONFIG_SYS_BOOTMAPSZ			(8<<20)

/*
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SKIP_RELOCATE_UBOOT
*/

#define DEBUG
#define CONFIG_SYS_HZ					1000
#define CONFIG_SYS_TIMERBASE			0xf1020300

/*-----------------------------------------------------------------------
 * Board-specific values for Orion5x MPP low level init:
 */
#define ORION5X_MPP0_7					0x00000000
#define ORION5X_MPP8_15					0x00000000
#define ORION5X_MPP16_23				0x00000000

/*
 * Board-specific values for Orion5x GPIO low level init:
 */
#define ORION5X_GPIO_OUT_ENABLE			0x00000126
#define ORION5X_GPIO_OUT_VALUE			0x00000000
#define ORION5X_GPIO_IN_POLARITY		0x00000000


/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS			1	   /* we have 1 bank of DRAM */
#define CONFIG_SYS_FLASH_BASE			0xfff80000

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

#define CONFIG_SYS_MAX_FLASH_BANKS		1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT		(135)	/* max number of sectors on one chip */

#define CONFIG_ENV_ADDR					0xfff80000
#define CONFIG_ENV_SIZE					0x2000
#define CONFIG_ENV_SECT_SIZE			0x2000

#define CONFIG_SYS_FLASH_CFI			1       /* Flash memory is CFI compliant */
#define CONFIG_FLASH_CFI_DRIVER			1       /* Use drivers/cfi_flash.c */
#define CONFIG_SYS_FLASH_CFI_WIDTH             FLASH_CFI_8BIT
#define CONFIG_SYS_FLASH_EMPTY_INFO

#define CONFIG_SYS_MAX_NAND_DEVICE		1
#define SECTORSIZE 512
#define NAND_NO_RB

#define ADDR_COLUMN 1
#define ADDR_PAGE 2
#define ADDR_COLUMN_PAGE 3

/*-----------------------------------------------------------------------
 * Network
 */
#ifdef CONFIG_CMD_NET
#define CONFIG_MVGBE				/* Enable Marvell GbE Driver */
#define CONFIG_MVGBE_PORTS				{1}		/* enable port 0 only */
#define CONFIG_SKIP_LOCAL_MAC_RANDOMIZATION	/* don't randomize MAC */
#define CONFIG_PHY_BASE_ADR				0x8
#define CONFIG_RESET_PHY_R	/* use reset_phy() to init mv8831116 PHY */
#define CONFIG_NETCONSOLE	/* include NetConsole support   */
#define CONFIG_NET_MULTI	/* specify more that one ports available */
#define	CONFIG_MII		/* expose smi ove miiphy interface */
#define CONFIG_SYS_FAULT_ECHO_LINK_DOWN	/* detect link using phy */
#define CONFIG_ENV_OVERWRITE	/* ethaddr can be reprogrammed */
#endif

/*-----------------------------------------------------------------------
 * Size of malloc() pool
 */
#define CONFIG_SYS_GBL_DATA_SIZE		128		/* size in bytes reserved for initial data */

#define CONFIG_SYS_MALLOC_LEN			(1024 * 1024)
#define CONFIG_SYS_MONITOR_LEN			(252 * 1024)
#define CONFIG_SYS_MONITOR_BASE			(CONFIG_SYS_FLASH_BASE)

/*-----------------------------------------------------------------------
 * Serial
 */
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE		-4               /* NS16550 register size */
#define CONFIG_SYS_NS16550_COM1			ORION5X_UART0_BASE      /* Base address of UART0 */
#define CONFIG_SYS_NS16550_CLK			CONFIG_SYS_TCLK          /* Input clock to NS16550 */
#define CONFIG_CONS_INDEX				1               /* use UART0 for console */
#define CONFIG_BAUDRATE					115200          /* Default baud rate */
#define CONFIG_SYS_BAUDRATE_TABLE		{ 9600, 19200, 38400, 57600, 115200, 230400 }

/*
#define CONFIG_BOOTCOMMAND "cp.b 0xff9a0000 0x400000 0x60d653;bootm 0xff820000 0x400000"
#define CONFIG_BOOTARGS "root=/dev/ram console=ttyS0,115200 :::DB88FXX81:egiga0:none"
*/

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_SYS_LONGHELP
#define	CONFIG_SYS_CBSIZE				256		/* Console I/O Buffer Size	*/
#define	CONFIG_SYS_PBSIZE				(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) /* Print Buffer Size */
#define	CONFIG_SYS_MAXARGS				16		/* max number of command args	*/
#define CONFIG_SYS_BARGSIZE				CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size	*/

#define CONFIG_SYS_MEMTEST_START		0x00400000
#define CONFIG_SYS_MEMTEST_END			0x00800000

#define CONFIG_SYS_ALT_MEMTEST
#define	CONFIG_SYS_LOAD_ADDR			0x00400000	/* default load address	*/

#undef  CONFIG_SYS_CLKS_IN_HZ		/* everything, incl board info, in Hz */

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ			(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ			(4*1024)	/* FIQ stack */
#endif

/*
 * Other required minimal configurations
 */
#define CONFIG_ARCH_CPU_INIT		/* call arch_cpu_init() */

#define CONFIG_SYS_RESET_ADDRESS		0xffff0000

#define CONFIG_SYS_SDRAM_BASE		0
#define CONFIG_SYS_INIT_SP_ADDR	\
	(CONFIG_SYS_SDRAM_BASE + 0x1000 - GENERATED_GBL_DATA_SIZE)

#endif	/* __CONFIG_H */
