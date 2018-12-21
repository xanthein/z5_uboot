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
#include <asm/arch/orion5x.h>
#include <asm/arch/cpu.h>
#include <spl.h>
#include "z5.h"

#define ORION5X_REG_WRITE(a,d) (*(volatile unsigned int*)(ORION5X_REGS_PHY_BASE | a) = (unsigned int)d)
#define ORION5X_REG_READ(a) (*(volatile unsigned int*)(ORION5X_REGS_PHY_BASE | a))

static inline void delay (unsigned long loops)
{
	__asm__ volatile ("1:\n"
		"subs %0, %1, #1\n"
		"bne 1b":"=r" (loops):"0" (loops));
}

DECLARE_GLOBAL_DATA_PTR;

static void z5_init_pci( void );
static void z5_init_devices( void );
void orion5x_pci_hw_rd_conf(int bus, int dev, u32 func,	u32 where, u32 *val);
void orion5x_pci_hw_wr_conf(int bus, int dev, u32 func,	u32 where, u32 val);

int board_init(void)
{
	int i;
	
	/* init serial */
	gd->baudrate = CONFIG_BAUDRATE;
	gd->have_console = 1;
	//serial_init();

	z5_init_devices();
	//z5_remap_registers();
	z5_init_pci();
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

	u32 p2p_config;
	p2p_config = ORION5X_REG_READ(0x31d14);
	int bus = (p2p_config & 0xff<<16)>>16;
	int dev = (p2p_config & 0x1f<<24)>>24;

	u32 temp;
	/* PCI Slave Address Decoding Registers */
	orion5x_pci_hw_rd_conf(bus, dev, 0, 0x20, &temp); /* Internal Reg Mem */
	temp &= ~(0xfffff << 12);
	temp |= ORION5X_REGS_PHY_BASE;
	orion5x_pci_hw_wr_conf(bus, dev, 0, 0x20, temp);

	orion5x_pci_hw_rd_conf(bus, dev, 4, 0x24, &temp); /* Internal Reg IO */
	temp &= ~(0xfffff << 12);
	temp |= ORION5X_REGS_PHY_BASE;
	orion5x_pci_hw_wr_conf(bus, dev, 4, 0x24, temp);

	ORION5X_REG_WRITE(0x30c08, 0x03fff000); /* CSn[0] BAR Size, 64MB */

	ORION5X_REG_WRITE(0x30c10, ORION5X_SZ_DEV_CS0); /* DevCSn[0] BAR Size, 2MB */
	ORION5X_REG_WRITE(0x30c50, ORION5X_ADR_DEV_CS0);
	orion5x_pci_hw_rd_conf(bus, dev, 2, 0x10, &temp);
	temp &= ~(0xfffff << 12);
	temp |= ORION5X_ADR_DEV_CS0;
	orion5x_pci_hw_wr_conf(bus, dev, 2, 0x10, temp);

	ORION5X_REG_WRITE(0x30d10, ORION5X_SZ_DEV_CS1); /* DevCSn[1] BAR Size, 8MB */
	ORION5X_REG_WRITE(0x30d50, ORION5X_ADR_DEV_CS1);
	orion5x_pci_hw_rd_conf(bus, dev, 2, 0x18, &temp);
	temp &= ~(0xfffff << 12);
	temp |= ORION5X_ADR_DEV_CS1;
	orion5x_pci_hw_wr_conf(bus, dev, 2, 0x18, temp);

	ORION5X_REG_WRITE(0x30d18, ORION5X_SZ_DEV_CS2); /* DevCSn[2] BAR Size, 1MB */
	ORION5X_REG_WRITE(0x30d58, ORION5X_ADR_DEV_CS2);
	orion5x_pci_hw_rd_conf(bus, dev, 2, 0x20, &temp);
	temp &= ~(0xfffff << 12);
	temp |= ORION5X_ADR_DEV_CS2;
	orion5x_pci_hw_wr_conf(bus, dev, 2, 0x20, temp);

	ORION5X_REG_WRITE(0x30d14, ORION5X_SZ_BOOTROM); /* BootRom BAR Size, 8MB */
	ORION5X_REG_WRITE(0x30d54, ORION5X_ADR_BOOTROM);
	orion5x_pci_hw_rd_conf(bus, dev, 3, 0x18, &temp);
	temp &= ~(0xfffff << 12);
	temp |= ORION5X_ADR_BOOTROM;
	orion5x_pci_hw_wr_conf(bus, dev, 3, 0x18, temp);

	ORION5X_REG_WRITE(0x30c3c, 0xffff<<16 | 0x3<<14 | 1<<11 | 1<<10 | 1<<3 | 1<<2 | 1<<1); /* Enable Base Address Registers */
	delay(48);

	u32 pci_status_cmd;
	orion5x_pci_hw_rd_conf(bus, dev, 0, 0x4, &pci_status_cmd);
	pci_status_cmd |= 1<<1 | 1<<2 | 1<<4 | 1<<7;
	orion5x_pci_hw_wr_conf(bus, dev, 0, 0x4, pci_status_cmd);

	u32 pcix_status;
	orion5x_pci_hw_rd_conf(bus, dev, 0, 0x64, &pcix_status);
	pcix_status &= ~(0xff << 8);
	pcix_status |= 1 << 8;
	pcix_status &= ~(0x1f << 3);
	pcix_status |= 0x0 << 3;
	orion5x_pci_hw_wr_conf(bus, dev, 0, 0x64, pcix_status);
}

static void z5_init_devices( void )
{
	/* Setup CS Magic Bits */
	ORION5X_REG_WRITE(0x1045c, 0x8fcfffff); /* Device CS0, SRAM */
	ORION5X_REG_WRITE(0x10460, 0x8fefffff); /* Device CS1, Flash */
	ORION5X_REG_WRITE(0x10464, 0x8fcfffff); /* Device CS2, UART */
	ORION5X_REG_WRITE(0x1046c, 0x8fcfffff); /* Bootdev CS */
}

void orion5x_pci_hw_rd_conf(int bus, int dev, u32 func,
					u32 where, u32 *val)
{
	ORION5X_REG_WRITE(0x30c78, PCI_CONF_BUS(bus) |
		PCI_CONF_DEV(dev) | PCI_CONF_REG(where) |
		PCI_CONF_FUNC(func) | PCI_CONF_ADDR_EN);

	*val = ORION5X_REG_READ(0x30c7c);
}

void orion5x_pci_hw_wr_conf(int bus, int dev, u32 func,
					u32 where, u32 val)
{
	ORION5X_REG_WRITE(0x30c78, PCI_CONF_BUS(bus) |
		PCI_CONF_DEV(dev) | PCI_CONF_REG(where) |
		PCI_CONF_FUNC(func) | PCI_CONF_ADDR_EN);

	ORION5X_REG_WRITE(0x30c7c, val);
}

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

/*
 * SPL serial setup and NOR boot device selection
 */

#ifdef CONFIG_SPL_BUILD

void spl_board_init(void)
{
	preloader_console_init();
}

u32 spl_boot_device(void)
{
	return BOOT_DEVICE_NOR;
}

#endif /* CONFIG_SPL_BUILD */
