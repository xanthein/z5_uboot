/*
 * (C) Copyright 2008 Byron Bradley <byron.bbradley@gmail.com>
 *
 * Modified for DNS-323 from Kuro Pro
 * (C) Copyright 2008 Veli-Matti Valtonen <maligor@gmail.com>
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
#include <miiphy.h>
#include "z5.h"

#define ORION5X_REGS_PHYS_BASE	0xf1000000
#define ORION5X_REG_WRITE(a,d) (*(volatile unsigned int*)(ORION5X_REGS_PHYS_BASE | a) = (unsigned int)d)

static inline void delay (unsigned long loops)
{
	__asm__ volatile ("1:\n"
		"subs %0, %1, #1\n"
		"bne 1b":"=r" (loops):"0" (loops));
}

DECLARE_GLOBAL_DATA_PTR;

static void z5_init_pci( void );
static void z5_init_devices( void );
static void z5_remap_registers( void );
static void z5_disable_cpu_streaming( void );

int board_init(void)
{
	int i;
	
	/* init serial */
	gd->baudrate = CONFIG_BAUDRATE;
	gd->have_console = 1;
	//serial_init();

	//z5_init_devices();
	//z5_remap_registers();
	//z5_init_pci();
	//z5_disable_cpu_streaming();

	/* arch number of DNS323 */
	//gd->bd->bi_arch_number = MACH_TYPE_DNS323;
	gd->bd->bi_arch_number = 526;

	/* adress of boot parameters */
        gd->bd->bi_boot_params = 0x00000100;


	/* relocate exception vectors */
	for( i = 0; i < 0x100; i+=4) {
		*(unsigned int *)(0x0 + i) = *(unsigned int*)(0xfff90000 +i );
	}

	return 0;
}
#if 0
int dram_init (void)
{
	gd->bd->bi_dram[0].start = 0x0;
	gd->bd->bi_dram[0].size = 0x4000000; /* 64MB */

	return 0;
}

int last_stage_init(void)
{
	return 0;
}

static void z5_init_pci( void )
{
	/* Configure PCI Express BARs */
	ORION5X_REG_WRITE(0x41804, 0x03FF0001); /* BAR1, 64MB */
	ORION5X_REG_WRITE(0x41808, 0x0FFF0000); /* BAR2, Disabled */

	delay(48);
	/* TODO: Do we need 0x418F8 and Interrupt Registers? */

	/* Configure PCI Express Windows */
	ORION5X_REG_WRITE(0x41820, 0x03ff0e01); /* Window0 Control, Enable, CS0, Bar1, 64MB */
	ORION5X_REG_WRITE(0x41824, 0x00000000); /* Window0 Base Address */
	/* All the rest are disabled and mapped to BAR2 */
	ORION5X_REG_WRITE(0x41830, 0x001f1e12); /* Window1 Disabled, DevCS0 */
	ORION5X_REG_WRITE(0x41834, 0xfa000000); /* Mapped to Encryption Engine SRAM */
	ORION5X_REG_WRITE(0x41840, 0x007f1d12); /* Window2 Disabled, DevCS1 */
	ORION5X_REG_WRITE(0x41844, 0xff800000); /* Mapped to flash */
	ORION5X_REG_WRITE(0x41850, 0x000f1b12); /* Window3 Disabled, DevCS2 */
	ORION5X_REG_WRITE(0x41854, 0xfa800000); /* Mapped to UART */
	ORION5X_REG_WRITE(0x41860, 0x007f0f12); /* Window4 Disabled, BootDevCS */
	ORION5X_REG_WRITE(0x41864, 0xff800000); /* Mapped to Flash */

	delay(48);
	/* Setup PCI */
	ORION5X_REG_WRITE(0x31d00, 0x80000030); /* PCI Arbiter - Enable */
	delay(48);
		
	/* PCI Slave Address Decoding Registers */
	ORION5X_REG_WRITE(0x30c08, 0x03fff000); /* CSn[0] BAR Size, 64MB */
	ORION5X_REG_WRITE(0x30c10, 0x001ff000); /* DevCSn[0] BAR Size, 2MB */
	ORION5X_REG_WRITE(0x30d10, 0x007ff000); /* DevCSn[1] BAR Size, 8MB */
	ORION5X_REG_WRITE(0x30d18, 0x000ff000); /* DevCSn[2] BAR Size, 1MB */
	ORION5X_REG_WRITE(0x30d14, 0x007ff000); /* DevCSn[3] BAR Size, 8MB */
	
	/* DevCS BAR Address */
	ORION5X_REG_WRITE(0x30c50, DEVICE_CS0_BASE); 
	ORION5X_REG_WRITE(0x30d50, DEVICE_CS1_BASE);
	ORION5X_REG_WRITE(0x30d58, DEVICE_CS2_BASE);
	ORION5X_REG_WRITE(0x30d54, BOOTDEV_CS_BASE);

	ORION5X_REG_WRITE(0x30c3c, 0xffff7c8e); /* Enable Base Address Registers */
	delay(48);
}

static void z5_init_devices( void )
{
#if 0 /* Moved to lowlevel init */
	/* MPP Config */
	ORION5X_REG_WRITE(0x10000, 0x00000000);
	ORION5X_REG_WRITE(0x10004, 0x00000000);
	ORION5X_REG_WRITE(0x10050, 0x00001111);
	/* Disable orange leds */
	ORION5X_REG_WRITE(0x10100, 0x00000006);
	/* GPIO Enable */
	ORION5X_REG_WRITE(0x10104, 0x000006e1);
	/* Enable blinking power led */
	ORION5X_REG_WRITE(0x10108, 0x00000108);
#endif

	/* Setup CS Magic Bits */
	ORION5X_REG_WRITE(0x1045c, 0x8fcfffff); /* Device CS0, SRAM */
	ORION5X_REG_WRITE(0x10460, 0x8fefffff); /* Device CS1, Flash */
	ORION5X_REG_WRITE(0x10464, 0x8fcfffff); /* Device CS2, UART */
	ORION5X_REG_WRITE(0x1046c, 0x8fcfffff); /* Bootdev CS */
}

static void z5_remap_registers( void )
{
	/* Remap Registers */
	ORION5X_REG_WRITE(0x20004, PEX0_MEM_BASE);
	ORION5X_REG_WRITE(0x20008, PEX0_MEM_REMAP);
	ORION5X_REG_WRITE(0x20000, PEX0_MEM_SIZE);
	ORION5X_REG_WRITE(0x20014, PCI0_MEM_BASE);
	ORION5X_REG_WRITE(0x20018, PCI0_MEM_REMAP);
	ORION5X_REG_WRITE(0x20010, PCI0_MEM_SIZE);

	ORION5X_REG_WRITE(0x20024, PEX0_IO_BASE);
	ORION5X_REG_WRITE(0x20020, PEX0_IO_SIZE);

	ORION5X_REG_WRITE(0x20034, PCI0_IO_BASE);
	ORION5X_REG_WRITE(0x20030, PCI0_IO_SIZE);

	ORION5X_REG_WRITE(0x20054, DEVICE_CS0_BASE);
	ORION5X_REG_WRITE(0x20050, DEVICE_CS0_SIZE);
	
	ORION5X_REG_WRITE(0x20064, DEVICE_CS1_BASE);
	ORION5X_REG_WRITE(0x20060, DEVICE_CS1_SIZE);

	ORION5X_REG_WRITE(0x20074, DEVICE_CS2_BASE);
	ORION5X_REG_WRITE(0x20070, DEVICE_CS2_SIZE);

	ORION5X_REG_WRITE(0x20044, BOOTDEV_CS_BASE);
	ORION5X_REG_WRITE(0x20040, BOOTDEV_CS_SIZE);
}

static void z5_disable_cpu_streaming( void )
{
	volatile unsigned int temp;

	__asm__ __volatile__("mrc    p15, 0, %0, c1, c0, 0" : "=r" (temp):: "memory");
	temp &= 0xefffffff;
	__asm__ __volatile__("mcr    p15, 0, %0, c1, c0, 0" : "=r" (temp):: "memory");
}
#endif

#if defined(CONFIG_CMD_NET) && defined(CONFIG_RESET_PHY_R)
/* Configure and enable MV88E1116 PHY */
void reset_phy(void)
{
	u16 reg;
	u16 devadr;
	char *name = "egiga0";

	if (miiphy_set_current_dev(name))
		return;

	/* command to read PHY dev address */
	if (miiphy_read(name, 0xEE, 0xEE, (u16 *) &devadr)) {
		printf("Err..%s could not read PHY dev address\n",
			__func__);
		return;
	}

	miiphy_read(name, devadr, MII_M1111_PHY_EXT_CR, &reg);
	reg |= (MII_M1111_RX_DELAY | MII_M1111_TX_DELAY);
	miiphy_write(name, devadr, MII_M1111_PHY_EXT_CR, reg);

	miiphy_read(name, devadr, MII_M1111_PHY_EXT_SR, &reg);
	reg &= ~(MII_M1111_HWCFG_MODE_MASK);
	reg |= MII_M1111_HWCFG_MODE_COPPER_RGMII;
	miiphy_write(name, devadr, MII_M1111_PHY_EXT_SR, reg);

	miiphy_write(name, devadr, MII_ADVERTISE, ADVERTISE_100FULL | ADVERTISE_100HALF | ADVERTISE_10FULL | ADVERTISE_10HALF);

	miiphy_write(name, devadr, MII_BMCR, BMCR_RESET);
	miiphy_write(name, devadr, MII_BMCR, BMCR_ANENABLE);

	/* reset the phy */
	miiphy_reset(name, devadr);

	printf("88E1111 Initialized on %s\n", name);
}
#endif /* CONFIG_RESET_PHY_R */
