/* Hey EMACS -*- linux-c -*- */
/* $Id: main.c 245 2004-05-23 20:45:43Z roms $ */

/*  TiEmu - an TI emulator
 *
 *  Copyright (c) 2000, Thomas Corvazier, Romain Lievin
 *  Copyright (c) 2001-2002, Romain Lievin, Julien Blache
 *  Copyright (c) 2003-2004, Romain Li�vin
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
    TI's ASIC management: glue logic for screen, keyboard, linkport, timers.
*/

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

#include "uae.h"
#include "dbus.h"
#include "ioports.h"
#include "callbacks.h"
#include "ti68k_def.h"

int cycle_instr = 640;
int cycle_count = 0;

/*
	Init hardware...
	A ROM image must have been loaded before calling this function.
*/

int hw_init(void)
{
	hw_mem_init();
	hw_io_init();
	hw_kbd_init();
	hw_timer_init();
	hw_lcd_init();
	hw_dbus_init();
	hw_m68k_init();
}

int hw_reset(void)
{
	hw_mem_reset();
	hw_io_reset();
	hw_kbd_reset();
	hw_timer_reset();
	hw_lcd_reset();
	hw_dbus_reset();
	hw_m68k_reset();
}

int hw_exit(void)
{
	hw_m68k_exit();
	hw_dbus_exit();
	hw_lcd_exit();
	hw_timer_exit();
	hw_kbd_exit();
	hw_io_exit();
	hw_mem_exit();
}

/* This function should be called everytime the counter increases */
void hw_update()
{
    /* Auto-int 5: timer */
    if(tihw.timer_value++ == 0)
    {
        tihw.timer_value = tihw.timer_init;
        specialflags |= SPCFLAG_INT;
        
        if(currIntLev < 5)
	        currIntLev = 5;
    }
    else
        tihw.timer_value &= 0xff;

    /* Auto-int 1: 1/4 of timer rate */
    if(!(tihw.timer_value&3)) 
    {
        specialflags |= SPCFLAG_INT;
        currIntLev = 1;
    }

  
    /* Auto-int 2: keyboard scan */
    if(!(tihw.timer_value & 2))
    {
    }

#if 0
	// Link status: error, link act, txbuf empty or rxbuf full
	io_bit_set(0x0d,6);						// txbuf always empty
	io_bit_chg(0x0d,5,hw_dbus_checkread());	// rxbuf full
	io_bit_chg(0x0d,3,hw_dbus_checkread());	// link activity

	// Trigger int4 on: error, link act, txbuf empty or rxbuf full
	if((io_bit_tst(0x0c,3) && io_bit_tst(0x0d,7)) ||
		(io_bit_tst(0x0c,2) && io_bit_tst(0x0d,2)) ||
		(io_bit_tst(0x0c,1) && io_bit_tst(0x0d,3)) ||
		(io_bit_tst(0x0c,0) && io_bit_tst(0x0d,5)))
    {
        specialflags |= SPCFLAG_INT;
        currIntLev = 4;
    }
#else
    /* Link status */
    if(hw_dbus_checkread())
        tihw.io[0xc] |= 2;

    /* Link interrupt */ 
    if(tihw.io[0xc] & 2) 
    {
        specialflags |= SPCFLAG_INT;
        currIntLev = 4;
    }
#endif
  
    /* LCD is refreshed every 16th time */
    if(!(tihw.timer_value&15))
    {
        if(tihw.lc_file) 
	    {
	        if(tihw.lc_timeout++ >= TO_VALUE) 
	        {
	            DISPLAY("Warning: internal link timeout !!!\n");
	            tihw.lc_file = 0;
	            tihw.lc_timeout = 0;
	        }
	    }

        hw_kbd_update();

        if(!params.sync_one)
	        cb_update_screen();
    }
}

