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

ulong board_flash_get_legacy (ulong base, int banknum, flash_info_t * info)
{
        if (banknum == 0) {
                info->portwidth = 2;
                info->chipwidth = 1;
                info->interface = FLASH_CFI_X8;
                return 1;
        } else
                return 0;
}
