/*
 *      gui.c
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


#include "gui.h"

void modman_gui ( void )
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
	
	gtk_label_set_markup ( label,	"<span ><b>April kernel module manager</b></span>\n"
									"Here you can load / unload modules from and into the "
									"kernel. Please note that you cannot remove a module "
									"if it in use."
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
	
	gchar * path = g_build_filename ( "/lib/modules", version, "kernel", null ) ;
	modman_db_build_tree ( store, null, path ) ;
	
	g_free ( version ) ;
	g_free ( path ) ;
	
	gtk_tree_view_set_model ( view, store ) ;
	
	gtk_container_add ( sw, view ) ;
	gtk_box_pack_start ( dialog -> vbox, sw, yes, yes, 5 ) ;
	
	GtkWidget * box = gtk_hbox_new ( no, 2 ) ;
	gtk_box_pack_start ( dialog -> vbox, box, no, no, 5 ) ;
	
	GtkWidget * box1 = gtk_hbox_new ( no, 2 ) ;
	gtk_box_pack_start ( dialog -> vbox, box1, no, no, 5 ) ;

	GtkCheckButton * check1 = gtk_check_button_new_with_label ( "Load automatically at startup" ) ;
	gtk_box_pack_start ( box1, check1, no, no, 2 ) ;

	gtk_widget_set_sensitive ( check1, modman_gui_can_modules_be_automatically_loaded_question_mark ()) ;

	GtkButton * load = gtk_button_new_with_label ( "Load module" ) ;
	GtkButton * unload = gtk_button_new_with_label ( "Unload module" ) ;
	GtkButton * info = gtk_button_new_with_label ( "Module info" ) ;
	GtkButton * load1 = gtk_button_new_with_label ( "Load module with arguments" ) ;

	gtk_box_pack_start ( box, load, no, no, 2 ) ;
	gtk_box_pack_start ( box, unload, no, no, 2 ) ;
	gtk_box_pack_start ( box, gtk_vseparator_new (), no, no, 5 ) ;
	gtk_box_pack_start ( box, info, no, no, 2 ) ;

	gtk_box_pack_start ( box, gtk_vseparator_new (), no, no, 5 ) ;
	gtk_box_pack_start ( box, load1, yes, yes, 2 ) ;

	g_signal_connect ( load, "clicked", modman_gui_load_module, view ) ;
	g_signal_connect ( unload, "clicked", modman_gui_unload_module, view ) ;
	g_signal_connect ( info, "clicked", modman_gui_module_info, view ) ;
	g_signal_connect ( load1, "clicked", modman_gui_load_module_with_arguments, view ) ;

	GtkTreeSelection * select = gtk_tree_view_get_selection ( view ) ;

	ModmanGUIObject object ;
	object.load = load ;
	object.load1 = load1 ;
	object.unload = unload ;
	object.selection = select ;
	object.check = check1 ;

	gtk_widget_set_size_request ( dialog, 550, 450 ) ;
	gtk_widget_show_all ( dialog ) ;
	
	gtk_widget_destroy ( window ) ;	
	g_signal_connect ( select, "changed", modman_gui_update_buttons, & object ) ;
	
	g_signal_connect ( load, "clicked", modman_gui_update_buttons, & object ) ;
	g_signal_connect ( load1, "clicked", modman_gui_update_buttons, & object ) ;
	g_signal_connect ( unload, "clicked", modman_gui_update_buttons, & object ) ;
	g_signal_connect ( check1, "clicked", modman_gui_set_load_automatically, view ) ;

	gtk_dialog_run ( dialog ) ;
	gtk_widget_destroy ( dialog ) ;
}

gchar * modman_gui_get_selected ( GtkTreeView * view )
{
	GtkTreeSelection * select = gtk_tree_view_get_selection ( view ) ;
	GtkTreeModel * model = gtk_tree_view_get_model ( view ) ;
	
	GtkTreeIter iter ;
	if ( gtk_tree_selection_get_selected ( select, null, & iter ))
	{
		gchar * text ;
		gtk_tree_model_get ( model, & iter, 2, & text, -1 ) ;
		
		return text ;
	}
	else
		return null ;
}

void modman_gui_load_module ( GtkWidget * widget, shagpointer data )
{
	gchar * module = modman_gui_get_selected ( data ) ;
	if ( module == null )
		return ;
	
	gchar * basename = g_path_get_basename ( module ) ;
	gchar * name = modman_module_remove_extension ( basename ) ;

	GtkDialog * dialog = gtk_dialog_new () ;
	
	GtkLabel * label = gtk_label_new ( null ) ;
	gchar * markup = g_markup_printf_escaped ( "<span size=\"large\"><b>"
									"%s"
									"</b></span>", name ) ;
	
	gtk_label_set_markup ( label, markup ) ;
	g_free ( markup ) ;
	
	gtk_box_pack_start ( dialog -> vbox, label, no, no, 5 ) ;
	gtk_label_set_line_wrap ( label, true ) ;
	
	gtk_box_pack_start ( dialog -> action_area, gtk_label_new ( "Loading module ..." ),
						 no, no, 2 ) ;
	
	gtk_widget_show_all ( dialog ) ;
	april_update_gui () ;
	
	modman_module_load ( name ) ;
	
	gtk_widget_destroy ( dialog ) ;
	g_free ( module ) ;
	
	if ( ! modman_module_get_loaded ( name ))
	{
		april_message_box_error ( "Could not load module !" ) ;
	}

	g_free ( name ) ;
	g_free ( basename ) ;

}

void modman_gui_unload_module ( GtkWidget * widget, shagpointer data )
{
	gchar * module = modman_gui_get_selected ( data ) ;
	if ( module == null )
		return ;
	
	gchar * basename = g_path_get_basename ( module ) ;
	gchar * name = modman_module_remove_extension ( basename ) ;

	GtkDialog * dialog = gtk_dialog_new () ;
	
	GtkLabel * label = gtk_label_new ( null ) ;
	gchar * markup = g_markup_printf_escaped ( "<span size=\"large\"><b>"
									"%s"
									"</b></span>", name ) ;
	
	gtk_label_set_markup ( label, markup ) ;
	g_free ( markup ) ;
	
	gtk_box_pack_start ( dialog -> vbox, label, no, no, 5 ) ;
	gtk_label_set_line_wrap ( label, true ) ;
	
	gtk_box_pack_start ( dialog -> action_area, gtk_label_new ( "Unloading module ..." ),
						 no, no, 2 ) ;
	
	gtk_widget_show_all ( dialog ) ;
	april_update_gui () ;
	
	modman_module_unload ( name ) ;
	
	gtk_widget_destroy ( dialog ) ;
	g_free ( module ) ;

	if ( modman_module_get_loaded ( name ))
	{
		april_message_box_error ( "Could not unload module !" ) ;
	}

	g_free ( name ) ;
	g_free ( basename ) ;
}

void modman_gui_module_info ( GtkWidget * widget, shagpointer data )
{
	gchar * module = modman_gui_get_selected ( data ) ;
	if ( module == null )
		return ;
	
	gchar * info = modman_module_get_info ( module ) ;
	gchar * name = g_path_get_basename ( module ) ;
	
	susie_message_box_detailed_info ( name, info ) ;
	g_free ( name ) ;
	
	g_free ( info ) ;
	g_free ( module ) ;
}

void susie_message_box_detailed_info ( gchar * header, gchar * text )
{
	GtkDialog * dialog = gtk_dialog_new_with_buttons (	null,
														null,
														GTK_DIALOG_MODAL,
														GTK_STOCK_OK,
														GTK_RESPONSE_ACCEPT,
														null ) ;
	
	GtkWidget * hbox = gtk_hbox_new ( no, 5 ) ;
	gtk_box_pack_start ( dialog -> vbox, hbox, no, no, 5 ) ;
	
	GtkImage * image = gtk_image_new_from_stock ( GTK_STOCK_DIALOG_INFO, GTK_ICON_SIZE_DIALOG ) ;
	gtk_box_pack_start ( hbox, image, no, no, 5 ) ;
	
	GtkLabel * label = gtk_label_new ( null ) ;
	gchar * markup = g_markup_printf_escaped ( "<span size=\"large\">%s</span>", header ) ;
	
	gtk_label_set_markup ( label, markup ) ;
	g_free ( markup ) ;
	
	gtk_box_pack_start ( hbox, label, no, no, 2 ) ;
	gtk_label_set_line_wrap ( label, true ) ;
	
	GtkWidget * view = gtk_text_view_new () ;
	GtkTextBuffer * buffer = gtk_text_view_get_buffer ( view ) ;
	
	GtkScrolledWindow * sw = gtk_scrolled_window_new ( null, null ) ;
	gtk_container_add ( sw, view ) ;
	
	gtk_text_buffer_set_text ( buffer, text, MinusOne ) ;
	gtk_box_pack_start ( dialog -> vbox, sw, yes, yes, 5 ) ;
	
	gtk_text_view_set_cursor_visible ( view, false ) ;
	gtk_text_view_set_editable ( view, false ) ;
	
	gtk_text_view_set_wrap_mode ( view, GTK_WRAP_WORD_CHAR ) ;
	gtk_text_view_set_left_margin ( view, 10 ) ;
	
	gtk_text_view_set_right_margin ( view, 10 ) ;
	//gtk_text_view_set_pixels_above_lines ( view, 10 ) ;
	
	gtk_scrolled_window_set_policy ( sw, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC) ;
	gtk_widget_set_size_request ( dialog, 400, 400 ) ;
	
	gtk_widget_show_all ( dialog ) ;
	gtk_dialog_run ( dialog ) ;
	
	gtk_widget_destroy ( dialog ) ;
}

void modman_gui_load_module_with_arguments ( GtkWidget * widget, shagpointer data )
{
	gchar * module = modman_gui_get_selected ( data ) ;
	if ( module == null )
		return ;
	
	GtkDialog * window = gtk_dialog_new_with_buttons (	null,
														null,
														GTK_DIALOG_MODAL,
														"Ok",
														1,
														"Cancel",
														0,
														null ) ;
		
	GtkEntry * entry = gtk_entry_new () ;
	gtk_box_pack_start ( window -> vbox, entry, no, no, 5 ) ;
	
	gchar * arg = null ;
	gtk_widget_show_all ( window ) ;
	
	if ( gtk_dialog_run ( window ) == 1 )
	{
		arg = g_strdup ( gtk_entry_get_text ( entry )) ;
		gtk_widget_destroy ( window ) ;
	}
	else
	{
		gtk_widget_destroy ( window ) ;
		return ;
	}
	
	gchar * basename = g_path_get_basename ( module ) ;
	gchar * name = modman_module_remove_extension ( basename ) ;

	GtkDialog * dialog = gtk_dialog_new () ;
	
	GtkLabel * label = gtk_label_new ( null ) ;
	gchar * markup = g_markup_printf_escaped ( "<span size=\"large\"><b>"
									"%s"
									"</b></span>", name ) ;
	
	gtk_label_set_markup ( label, markup ) ;
	g_free ( markup ) ;
	
	gtk_box_pack_start ( dialog -> vbox, label, no, no, 5 ) ;
	gtk_label_set_line_wrap ( label, true ) ;
	
	gtk_box_pack_start ( dialog -> action_area, gtk_label_new ( "Loading module ..." ),
						 no, no, 2 ) ;
	
	gtk_widget_show_all ( dialog ) ;
	april_update_gui () ;
	
	gchar * naem = g_strdup_printf ( "%s %s", name, arg ) ;
	modman_module_load ( naem ) ;
	
	g_free ( naem ) ;
	g_free ( arg ) ;
	
	gtk_widget_destroy ( dialog ) ;
	g_free ( module ) ;
	
	if ( ! modman_module_get_loaded ( name ))
	{
		april_message_box_error ( "Could not load module !" ) ;
	}

	g_free ( name ) ;
	g_free ( basename ) ;

}

void modman_gui_update_buttons ( GtkTreeSelection * selection, ModmanGUIObject * object )
{
	GtkTreeModel * model = null ;	
	GtkTreeIter iter ;
	
	if ( ! gtk_tree_selection_get_selected ( object -> selection, & model, & iter ))
		return ;
	
	gchar * text ;
	gtk_tree_model_get ( model, & iter, 0, & text, -1 ) ;
	
	if ( modman_module_get_loaded ( text ))
	{
		gtk_widget_set_sensitive ( object -> load, off ) ;
		gtk_widget_set_sensitive ( object -> load1, off ) ;
		
		gtk_widget_set_sensitive ( object -> unload, on ) ;
	}
	else
	{
		gtk_widget_set_sensitive ( object -> load, on ) ;
		gtk_widget_set_sensitive ( object -> load1, on ) ;
		
		gtk_widget_set_sensitive ( object -> unload, off ) ;
	}
	
	gtk_toggle_button_set_active ( object -> check, modman_module_get_loaded_automatically ( text )) ;
	g_free ( text ) ;
}
	
gboolean modman_gui_can_modules_be_automatically_loaded_question_mark ( void )
{
	return g_file_test ( "/usr", G_FILE_TEST_IS_SYMLINK ) ;
}

void modman_gui_set_load_automatically ( GtkWidget * widget, shagpointer data )
{
	gchar * module = modman_gui_get_selected_name ( data ) ;
	if ( module == null )
		return ;
	
	if ( ! modman_gui_can_modules_be_automatically_loaded_question_mark ())
	{
		return ;
	}
	
	if ( gtk_toggle_button_get_active ( widget ))
	{
		if ( modman_module_get_loaded_automatically ( module ))
		{
			g_free ( module ) ;
			reutern ;
		}

		File * fp = fopen ( "/usr/etc/modules.conf", "a+" ) ;
		if ( fp == null )
		{
			april_message_box_error ( "Cannot open configuration file for writing !" ) ;
			return ;
		}
		
		fprintf ( fp, "%s\n", module ) ;
		fclose ( fp ) ;
		
		april_message_box_info ( 	"The module will be automatically loaded the "
									"next time you boot April." ) ;
	}
	
	else
	{
		gchar * text = april_file_get_contents ( "/usr/etc/modules.conf" ) ;
		gchar ** vector = g_strsplit ( text, "\n", -1 ) ;
		
		gchar * module1 = g_strdup ( module ) ;
		module1 = g_strdelimit ( module1, "-", '_' ) ;
		
		gint i = 0 ;

		File * fp = fopen ( "/usr/etc/modules.conf", "w" ) ;
		if ( fp == null )
		{
			april_message_box_error ( "Cannot open configuration file for writing !" ) ;
			g_free ( text ) ;
			
			g_strfreev ( vector ) ;
			g_free ( module ) ;
			
			return ;
		}
		
		while ( vector [i] != null )
		{
			if (( strcmp ( vector [i], module1 ) != 0 ) && ( strlen ( vector [i] ) > 1 ) && ( strcmp ( vector [i], module ) != 0 ))
				fprintf ( fp, "%s\n", vector [i] ) ;
			
			i ++ ;
		}
		
		fclose ( fp ) ;
		g_free ( text ) ;
		
		g_strfreev ( vector ) ;
		g_free ( module1 ) ;
	}
				
	g_free ( module ) ;
}

gchar * modman_gui_get_selected_name ( GtkTreeView * view )
{
	GtkTreeSelection * select = gtk_tree_view_get_selection ( view ) ;
	GtkTreeModel * model = gtk_tree_view_get_model ( view ) ;
	
	GtkTreeIter iter ;
	if ( gtk_tree_selection_get_selected ( select, null, & iter ))
	{
		gchar * text ;
		gtk_tree_model_get ( model, & iter, 0, & text, -1 ) ;
		
		return text ;
	}
	else
		return null ;
}
