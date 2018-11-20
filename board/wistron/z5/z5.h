#ifndef _Z5_H_
#define _Z5_H_

#define PEX0_MEM_REMAP
#define PEX0_MEM_SIZE
#define PCI0_MEM_BASE
#define PCI0_MEM_REMAP
#define PCI0_MEM_SIZE
#define PEX0_IO_BASE
#define PEX0_IO_SIZE
#define PCI0_IO_BASE
#define PCI0_IO_SIZE
#define DEVICE_CS0_BASE
#define DEVICE_CS0_SIZE
#define DEVICE_CS1_BASE
#define DEVICE_CS1_SIZE
#define DEVICE_CS2_BASE
#define DEVICE_CS2_SIZE
#define BOOTDEV_CS_BASE
#define BOOTDEV_CS_SIZE

#define PCI_CONF_REG(reg)               ((reg) & 0xfc)
#define PCI_CONF_FUNC(func)             (((func) & 0x3) << 8)
#define PCI_CONF_DEV(dev)               (((dev) & 0x1f) << 11)
#define PCI_CONF_BUS(bus)               (((bus) & 0xff) << 16)
#define PCI_CONF_ADDR_EN                (1 << 31)

#define MII_M1111_PHY_LED_CONTROL       0x18
#define MII_M1111_PHY_LED_DIRECT        0x4100
#define MII_M1111_PHY_LED_COMBINE       0x411c
#define MII_M1111_PHY_EXT_CR            0x14
#define MII_M1111_RX_DELAY              0x80
#define MII_M1111_TX_DELAY              0x2
#define MII_M1111_PHY_EXT_SR            0x1b
#define MII_M1111_HWCFG_MODE_MASK               0xf
#define MII_M1111_HWCFG_MODE_COPPER_RGMII       0xb
#define MII_M1111_HWCFG_MODE_FIBER_RGMII        0x3
#define MII_M1111_HWCFG_MODE_SGMII_NO_CLK       0x4
#define MII_M1111_HWCFG_MODE_COPPER_RTBI        0x9
#define MII_M1111_HWCFG_FIBER_COPPER_AUTO       0x8000
#define MII_M1111_HWCFG_FIBER_COPPER_RES        0x2000

#define MII_M1111_COPPER                0
#define MII_M1111_FIBER                 1

#endif
