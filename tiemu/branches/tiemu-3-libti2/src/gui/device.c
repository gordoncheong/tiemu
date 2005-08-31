/* Hey EMACS -*- linux-c -*- */
/* $Id: device.c 1364 2005-07-13 12:08:10Z roms $ */

/*  TiLP - Ti Linking Program
 *  Copyright (C) 1999-2005  Romain Lievin
 *
 *  This program is free software you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#undef GTK_DISABLE_DEPRECATED
#include <gtk/gtk.h>
#define GTK_DISABLE_DEPRECATED
#include <glade/glade.h>
#include <string.h>

#include "intl.h"
#include "support.h"
#include "paths.h"
#include "device.h"
#include "refresh.h"
#include "tilibs.h"
#include "dboxes.h"
#include "tie_error.h"

CableHandle* cable_handle = NULL;
CalcHandle*  calc_handle  = NULL;

extern DeviceOptions	link;
static DeviceOptions	tmp;

static GtkWidget* lbl;
static GtkWidget* om_cable;
static GtkWidget* om_calc;
static GtkWidget* om_port;

gint display_device_dbox()
{
	GladeXML *xml;
	GtkWidget *dbox;
	GtkWidget *data;
	gint result;
	int err;

	xml = glade_xml_new(tilp_paths_build_glade("device-2.glade"), "device_dbox", PACKAGE);
	if (!xml)
		g_error(_("comm.c: GUI loading failed !\n"));
	glade_xml_signal_autoconnect(xml);

	dbox = glade_xml_get_widget(xml, "device_dbox");

	lbl = glade_xml_get_widget(xml, "label7");

	// Cable  
	data = om_cable = glade_xml_get_widget(xml, "optionmenu_comm_cable");
	switch (link.cable_model) 
	{
	case CABLE_NUL:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 0);
	break;

	case CABLE_GRY:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 1);
	break;
		
	case CABLE_BLK:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 2);
	break;

	case CABLE_PAR:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 3);
	break;

	case CABLE_SLV:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 4);
	break;	

	case CABLE_USB:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 5);
	break;	

	case CABLE_VTI:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 6);
	break;

    case CABLE_TIE:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 7);
	break;
	
	case CABLE_VTL:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 8);
	break;   

	default:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 0);
	  break;
	}

	// Port
	data = om_port = glade_xml_get_widget(xml, "optionmenu_comm_port");
	switch (link.cable_port) 
	{
	case PORT_0:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 0);
	break;
	case PORT_1:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 1);
	break;
	case PORT_2:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 2);
	break;
	case PORT_3:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 3);
	break;
	case PORT_4:
		gtk_option_menu_set_history(GTK_OPTION_MENU(data), 4);
	break;
	}

	// Calc
	data = om_calc = glade_xml_get_widget(xml, "optionmenu_comm_calc");
	switch (link.calc_model) 
	{
	case CALC_NONE:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 0);
	  break;

	case CALC_TI73:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 1);
	  break;
	  
	case CALC_TI82:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 2);
	  break;

	case CALC_TI83:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 3);
	  break;
	  
	case CALC_TI83P:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 4);
	  break;

	case CALC_TI84P:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 5);
	  break;
	  
	case CALC_TI85:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 6);
	  break;
	  
	case CALC_TI86:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 7);
	  break;
	  
	case CALC_TI89:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 8);
	  break;

	case CALC_TI89T:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 9);
	  break;
	  
	case CALC_TI92:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 10);
	  break;
	  
	case CALC_TI92P:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 11);
	  break;
	  
	case CALC_V200:
	  gtk_option_menu_set_history(GTK_OPTION_MENU(data), 12);
	  break;
	}

	// Timeout
	data = glade_xml_get_widget(xml, "spinbutton_comm_timeout");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), link.cable_timeout);
	
	// Delay
	data = glade_xml_get_widget(xml, "spinbutton_comm_delay");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(data), link.cable_delay);

	// Avoid early callbacks
	tmp.cable_delay = link.cable_delay;
	tmp.cable_model = link.cable_model;
	tmp.cable_port = link.cable_port;
	tmp.cable_timeout = link.cable_timeout;
	tmp.calc_model = link.calc_model;

	// Close handles
	// detach cable (made by handle_del, too)
	err = ticalcs_cable_detach(calc_handle);
	if(tiemu_error(err, NULL))
		return -1;

	// remove calc & cable
	ticalcs_handle_del(calc_handle);
	ticables_handle_del(cable_handle);
	
	// Loop
 loop:
	result = gtk_dialog_run(GTK_DIALOG(dbox));
	switch (result) 
	{
	case GTK_RESPONSE_OK:
	case GTK_RESPONSE_CANCEL:
		// copy options
		cable_handle = ticables_handle_new(tmp.cable_model, tmp.cable_port);
		if(cable_handle == NULL)
		{
			msg_box1("Error", "Can't set cable");
			goto loop;
		}
		else
		{
			calc_handle = ticalcs_handle_new(tmp.calc_model);
			if(calc_handle == NULL)
			{
				msg_box1("Error", "Can't set cable");
				goto loop;
			}
			else
			{
				err = ticalcs_cable_attach(calc_handle, cable_handle);
				tiemu_error(err, NULL);
			}
			ticables_options_set_timeout(cable_handle, tmp.cable_timeout);
			ticables_options_set_delay(cable_handle, tmp.cable_delay);
		}

		link.cable_delay = tmp.cable_delay;
		link.cable_model = tmp.cable_model;
		link.cable_port = tmp.cable_port;
		link.cable_timeout = tmp.cable_timeout;
		link.calc_model = tmp.calc_model;

		break;
	case GTK_RESPONSE_HELP:
		break;
	default:
		break;
	}
	gtk_widget_destroy(dbox);
	return 0;
}


GLADE_CB void
comm_cable_changed                     (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
	gint nitem = gtk_option_menu_get_history(optionmenu);

	switch(nitem)
	{
	case 0: tmp.cable_model = CABLE_NUL; break;
	case 1: tmp.cable_model = CABLE_GRY; break;
	case 2: tmp.cable_model = CABLE_BLK; break;
	case 3:	tmp.cable_model = CABLE_PAR; break;
	case 4: tmp.cable_model = CABLE_SLV; break;	
	case 5: tmp.cable_model = CABLE_USB; break;	
	case 6: tmp.cable_model = CABLE_VTI; break;
	case 7: tmp.cable_model = CABLE_TIE; break;
	case 8: tmp.cable_model = CABLE_VTL; break;
	}
}

GLADE_CB void
comm_cable_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
}


GLADE_CB void
comm_port_changed                      (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
	GtkWidget *menu_item = optionmenu->menu_item;
	gchar *ed = menu_item->name;

	if(!strcmp(ed, "number0"))
		tmp.cable_port = PORT_0;
	else if(!strcmp(ed, "number1"))
		tmp.cable_port = PORT_1;
	else if(!strcmp(ed, "number2"))
		tmp.cable_port = PORT_2;
	else if(!strcmp(ed, "number3"))
		tmp.cable_port = PORT_3;
	else if(!strcmp(ed, "number4"))
		tmp.cable_port = PORT_4;
}

GLADE_CB void
comm_port_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
}


GLADE_CB void
comm_calc_changed                      (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
	gint nitem = gtk_option_menu_get_history(optionmenu);

	switch(nitem)
	{
	case 0: tmp.calc_model = CALC_NONE;	 break;
	case 1: tmp.calc_model = CALC_TI73;	 break;
	case 2:	tmp.calc_model = CALC_TI82;  break;
	case 3: tmp.calc_model = CALC_TI83;  break;
	case 4: tmp.calc_model = CALC_TI83P; break;
	case 5: tmp.calc_model = CALC_TI84P; break;
	case 6: tmp.calc_model = CALC_TI85;  break;
	case 7: tmp.calc_model = CALC_TI86;  break;
	case 8: tmp.calc_model = CALC_TI89;  break;
	case 9:tmp.calc_model = CALC_TI89T; break;
	case 10:tmp.calc_model = CALC_TI92;  break;
	case 11:tmp.calc_model = CALC_TI92P; break;
	case 12:tmp.calc_model = CALC_V200;  break;
  	}	
}

GLADE_CB void
comm_spinbutton_delay_changed          (GtkEditable     *editable,
                                        gpointer         user_data)
{
  	tmp.cable_delay =
    		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(user_data));
}


GLADE_CB void
comm_spinbutton_timeout_changed        (GtkEditable     *editable,
                                        gpointer         user_data)
{
  	tmp.cable_timeout =
    		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(user_data));
}