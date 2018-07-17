/*
 *      answer_the_call.c
 *
 *      Copyright 2009 Shaji <djshaji@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/*		Answer the call, hereafter referred to as atc */

#include "answer_the_call.h"

gint32 modman_answer_the_call_get_max_freq ( void )
{
	gchar * text = null ;
	g_file_get_contents ( "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", & text, null, null ) ;
	
	gint32 freq = 0 ;
	
	if ( text != null )
		freq = atoi ( text ) ;
	
	g_free ( text ) ;
	return freq ;
}

gint32 modman_answer_the_call_get_min_freq ( void )
{
	gchar * text = null ;
	g_file_get_contents ( "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq", & text, null, null ) ;
	
	gint32 freq = 0 ;

	if ( text != null )
		freq = atoi ( text ) ;
		
	g_free ( text ) ;
	return freq ;
}

gint32 modman_answer_the_call_get_cur_freq ( void )
{
	gchar * text = null ;
	g_file_get_contents ( "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", & text, null, null ) ;
	
	gint32 freq = 0 ;

	if ( text != null )
		freq = atoi ( text ) ;
	
	g_free ( text ) ;
	return freq ;
}

void modman_answer_the_call ( void )
{
	GtkSettings * settings = gtk_settings_get_default () ;
	gtk_settings_set_long_property ( settings, "gtk-button-images", false, "XProperty" ) ;

	GtkDialog * window = gtk_dialog_new () ;
	GtkWidget * hbox = gtk_hbox_new ( no, 2 ) ;
	
	gtk_box_pack_start ( window -> vbox, hbox, no, no, 5 ) ;
	gtk_box_pack_start ( hbox, gtk_label_new ( "Gathering required information ..." ), no, no, 5 ) ;
	
	gtk_box_pack_start ( window -> action_area, gtk_label_new ( "Oceania 1.7" ), no, no, 2 ) ;
	g_signal_connect ( window, "delete-event", gtk_true, null ) ;

	gtk_widget_show_all ( window ) ;
	april_update_gui () ;

	GtkDialog * dialog = gtk_dialog_new_with_buttons (	null,
														null,
														GTK_DIALOG_MODAL,
														GTK_STOCK_CLOSE,
														GTK_RESPONSE_ACCEPT,
														null ) ;
	
	//gtk_box_pack_end ( dialog -> action_area, gtk_label_new ( "djshaji@gmail.com" ), no, no, 5 ) ;
	
	GtkScrolledWindow * sw = gtk_scrolled_window_new ( null, null ) ;
	GtkTreeView * view = gtk_tree_view_new () ;
	
	gtk_tree_view_set_headers_visible ( view, no ) ;
	
	GtkBox * header = gtk_hbox_new ( no, 2 ) ;
	gtk_box_pack_start ( dialog -> vbox, header, no, no, 5 ) ;
	
	GdkPixbuf * icon = gtk_icon_theme_load_icon (	gtk_icon_theme_get_default (),
													"gnome-settings",
													64,
													GTK_ICON_LOOKUP_NO_SVG,
													null ) ;
	GtkLabel * label = gtk_label_new ( nothing ) ;
	
	gtk_label_set_markup ( label,	"<span ><b>April Processor Clock Modulator</b></span>\n"
									"Here you can set the speed of your processor "
									"to an optimally useful level."
									 ) ;
	gtk_label_set_line_wrap ( label, on ) ;
	
	gtk_box_pack_start ( header, gtk_image_new_from_pixbuf ( icon ), no, no, 5 ) ;
	gtk_box_pack_start ( header, label, no, no, 5 ) ;
									
	
	GtkCellRenderer * cell1 = gtk_cell_renderer_text_new () ;
	GtkCellRenderer * cell2 = gtk_cell_renderer_text_new () ;
	
	gtk_scrolled_window_set_policy ( sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC ) ;

	GtkTreeViewColumn * column1 = gtk_tree_view_column_new_with_attributes ( "Name",
																			cell1,
																			"text",
																			0,
																			null ) ;
	
	GtkTreeViewColumn * column2 = gtk_tree_view_column_new_with_attributes ( "Description",
																			cell2,
																			"text",
																			1,
																			null ) ;

	gtk_tree_view_append_column ( view, column1 ) ;
	//gtk_tree_view_append_column ( view, column2 ) ;
	
	GtkTreeStore * store = gtk_tree_store_new ( 3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING ) ;
	gchar * version = modman_module_get_kernel_version () ;

	GtkTreeIter iter ;
	gtk_tree_store_append ( store, & iter, null ) ;
	
	gtk_tree_store_set ( store, & iter, 0, "Select a driver", 1, "Select a driver", -1 ) ;
	
	gchar * path = g_build_filename ( "/lib/modules", version, "kernel", "arch/i386/kernel/cpu/cpufreq", null ) ;
	modman_db_find_modules ( store, & iter, path ) ;
	
	g_free ( version ) ;
	g_free ( path ) ;
	
	gtk_tree_view_set_model ( view, store ) ;	
	gtk_container_add ( sw, view ) ;

	GtkWidget * box = gtk_hbox_new ( no, 2 ) ;
	gtk_box_pack_start ( dialog -> vbox, box, yes, yes, 5 ) ;
	
	gtk_box_pack_start ( box, sw, yes, yes, 5 ) ;
	
	GtkVBox * vbox = gtk_vbox_new ( no, 2 ) ;
	gtk_box_pack_start ( box, vbox, no, no, 5 ) ;
	
	if ( ! g_file_test ( "/sys/devices/system/cpu/cpu0/cpufreq/scaling_driver", G_FILE_TEST_EXISTS ))
		gtk_widget_set_sensitive ( vbox, false ) ;
	else
		gtk_widget_set_sensitive ( view, false ) ;
	
	GtkWidget * box1 = gtk_hbox_new ( no, 2 ) ;
	gtk_box_pack_start ( vbox, box1, no, no, 5 ) ;
	
	gint	min = modman_answer_the_call_get_min_freq (),
			max = modman_answer_the_call_get_max_freq () ;
	
	GtkSpinButton * spin = gtk_spin_button_new_with_range (( gdouble ) min, ( gdouble ) max, 1.0 ) ;
	
	gtk_box_pack_start ( box1, gtk_label_new ( "Choose speed :" ), no, no, 2 ) ;
	gtk_box_pack_end ( box1, spin, no, no, 2 ) ;
	
	GtkWidget * box2 = gtk_hbox_new ( no, 2 ) ;
	gtk_box_pack_start ( vbox, box2, no, no, 5 ) ;
	
	GtkButton * set_max = gtk_button_new_with_label ( "Maximum" ),
			  * set_min = gtk_button_new_with_label ( "Minimum" ), 
			  * set_cur = gtk_button_new_with_label ( "Current" ) ;
	
	gtk_box_pack_start ( box2, set_min, no, no, 2 ) ;
	gtk_box_pack_start ( box2, set_max, no, no, 2 ) ;
	gtk_box_pack_start ( box2, set_cur, no, no, 2 ) ;
	
	GtkButton * button = gtk_button_new_with_label ( "Set CPU Speed" ) ;
	gtk_box_pack_start ( vbox, button, no, no, 2 ) ;
	
	GtkLabel * label1 = gtk_label_new ( null ) ;
	gtk_label_set_line_wrap ( label1, on ) ;
	
	gtk_box_pack_start ( dialog -> vbox, label1, no, no, 10 ) ;

	ModmanAnswerTheCallObject object ;
	object.spin = spin ;
	object.view = view ;
	object.label = label1 ;
	object.box = vbox ;
	object.button = set_cur ;
	
	g_signal_connect ( button, "clicked", modman_answer_the_call_set_speed, & object ) ;
	g_signal_connect ( set_min, "clicked", modman_answer_the_call_set_min_speed, spin ) ;
	
	g_signal_connect ( set_max, "clicked", modman_answer_the_call_set_max_speed, spin ) ;
	g_signal_connect ( set_cur, "clicked", modman_answer_the_call_set_cur_speed, spin ) ;

	g_signal_connect ( button, "clicked", modman_answer_the_call_set_cur_speed, spin ) ;
	GtkTreeSelection * selection = gtk_tree_view_get_selection ( view ) ;
	
	g_signal_connect ( selection, "changed", modman_answer_the_call_update_info, & object ) ;
	gtk_widget_set_size_request ( dialog, 450, 450 ) ;
	
	g_signal_connect ( view, "row-activated", modman_answer_the_call_driver_selected, & object ) ; 

	GtkFrame * frame = gtk_frame_new ( "CPU Speed Governor" ) ;
	gtk_box_pack_start ( vbox, frame, no, no, 5 ) ;
	
	GtkWidget * box9 = gtk_hbox_new ( no, 2 ) ;
	gtk_container_add ( frame, box9 ) ;
	
	GtkComboBox * combo = gtk_combo_box_new_text () ;
	gchar * current = modman_answer_the_call_governer_get () ;
	
	gint i = 0 ;
	while ( cpu_governors [i] != null )
	{
		gtk_combo_box_append_text ( combo, cpu_governors [i] ) ;
		if ( g_strrstr ( current, cpu_governors [i] ) != null )
			gtk_combo_box_set_active ( combo, i ) ;
		
		i ++ ;
	}
	
	GtkButton * set = gtk_button_new_with_label ( "Set Governor" ) ;
	g_signal_connect_swapped ( set, "clicked", modman_answer_the_call_governor_callback , combo ) ;
	
	gtk_box_pack_start ( box9, combo, yes, yes, 5 ) ;
	gtk_box_pack_start ( box9, set, no, no, 2 ) ;
	
	gtk_button_clicked ( set_cur ) ;
	g_free ( current ) ;
	
	gtk_widget_show_all ( dialog ) ;
	gtk_widget_destroy ( window ) ;
	
	gtk_dialog_run ( dialog ) ;	
	gtk_widget_destroy ( dialog ) ;
}

void modman_answer_the_call_set_governor ( vodi )
{
	if ( ! modman_module_get_loaded ( "cpufreq_userspace" ))
		modman_module_load ( "cpufreq_userspace" ) ;
	
	system ( "echo userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor" ) ;
}

gboolean modman_answer_the_call_get_governor ( void )
{
	gchar * text = april_file_get_contents ( "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor" ) ;
	gboolean result = false ;
	
	if ( g_strrstr ( text, "userspace" ) != null )
		result = true ;

	g_free ( text ) ;
	return result ;

}

void modman_answer_the_call_set_speed ( GtkWidget * widget, ModmanAnswerTheCallObject * data )
{
	GtkTreeSelection * selection = gtk_tree_view_get_selection ( data -> view ) ;
	GtkTreeModel * model = gtk_tree_view_get_model ( data -> view ) ;
	
	GtkTreeIter iter ;
	
	if ( ! modman_answer_the_call_driver_present ())
	{
		if ( ! gtk_tree_selection_get_selected ( selection, null, & iter ))
			return ;
	}
	
	if ( ! modman_answer_the_call_driver_present ())
	{
		if ( gtk_tree_model_iter_n_children ( model, & iter ) != 0 )
		{
			april_message_box_info ( "You have to select the correct driver "
									 "for your processor from the list given." ) ;
			return ;
		}
	}
	
	if ( ! modman_answer_the_call_get_governor ())
	{
		modman_answer_the_call_set_governor () ;
	}
	
	if ( ! modman_answer_the_call_driver_present ())
	{
		gchar * module = null ;
		gtk_tree_model_get ( model, & iter, 0, & module, -1 ) ;
	
		if ( ! modman_module_get_loaded ( module ))
			modman_module_load ( module ) ;

		g_free ( module ) ;
	}
	
	gint speed = gtk_spin_button_get_value_as_int ( data -> spin ) ;
	gchar * command = g_strdup_printf ( "echo %d > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed", speed ) ;
	
	system ( command ) ;		
	g_free ( command ) ;

	april_message_box_info ( "Speed has been set." ) ;
}

void modman_answer_the_call_set_max_speed ( GtkWidget * widget, shagpointer data )
{
	gtk_spin_button_set_value ( data, ( gdouble ) modman_answer_the_call_get_max_freq ()) ;
}

void modman_answer_the_call_set_min_speed ( GtkWidget * widget, shagpointer data )
{
	gtk_spin_button_set_value ( data, ( gdouble ) modman_answer_the_call_get_min_freq ()) ;
}

void modman_answer_the_call_set_cur_speed ( GtkWidget * widget, shagpointer data )
{
	gtk_spin_button_set_value ( data, ( gdouble ) modman_answer_the_call_get_cur_freq ()) ;
}

void modman_answer_the_call_update_info ( GtkWidget * widget, ModmanAnswerTheCallObject * data )
{
	GtkTreeSelection * selection = gtk_tree_view_get_selection ( data -> view ) ;
	GtkTreeModel * model = gtk_tree_view_get_model ( data -> view ) ;
	
	GtkTreeIter iter ;
	if ( ! gtk_tree_selection_get_selected ( selection, null, & iter ))
		return ;
	
	if ( gtk_tree_model_iter_n_children ( model, & iter ) != 0 )
	{
		return ;
	}
	
	gchar * module = null ;
	gtk_tree_model_get ( model, & iter, 1, & module, -1 ) ;
	
	gtk_label_set_text ( data -> label, module ) ;
	g_free ( module ) ;
}

void modman_answer_the_call_driver_selected ( GtkWidget * widget, GtkTreePath * path, GtkTreeViewColumn * column, ModmanAnswerTheCallObject * data )
{
	GtkTreeIter iter ;
	GtkTreeModel * model = gtk_tree_view_get_model ( widget ) ;
	
	gtk_tree_model_get_iter ( model, & iter, path ) ;
	
	gchar * module = null ;
	gtk_tree_model_get ( model, & iter, 0, & module, -1 ) ;
	
	if ( ! modman_module_get_loaded ( module ))
		modman_module_load ( module ) ;
	
	if ( ! GTK_WIDGET_SENSITIVE ( data -> box ))
	{
		gtk_widget_set_sensitive ( data -> box, true ) ;
		gtk_button_clicked ( data -> button ) ;
	
		gtk_spin_button_set_range (	data -> spin,
									modman_answer_the_call_get_min_freq (),
									modman_answer_the_call_get_max_freq ()) ;
	}
	
	g_free ( module ) ;
}

gboolean modman_answer_the_call_driver_present ( void )
{
	return g_file_test ( "/sys/devices/system/cpu/cpu0/cpufreq/scaling_driver", G_FILE_TEST_EXISTS ) ;
}

void modman_answer_the_call_governer_set ( gchar * governor )
{
	gchar * command = g_strdup_printf ( "echo %s > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", governor ) ;
	system ( command ) ;
	
	g_free ( command ) ;
}

gchar * modman_answer_the_call_governer_get ( void )
{
	gchar * text = null ;
	text = april_file_get_contents ( "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"  ) ;
	
	return text ;
}

void modman_answer_the_call_governor_callback ( GtkWidget * widget, gpointer data )
{
	gchar * governor = gtk_combo_box_get_active_text ( widget ) ;
	modman_answer_the_call_governer_set ( governor ) ;
	
	gchar * new = modman_answer_the_call_governer_get () ;
	if ( g_strrstr ( new, governor ) == null )
		april_message_box_warning ( "Could not set governor !" ) ;
	else
		april_message_box_info ( "Governor has been set." ) ;
		
	g_free ( new ) ;
	g_free ( governor ) ;
}
