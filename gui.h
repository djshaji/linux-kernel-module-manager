/*
 *      gui.h
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

#include <april/april.h>

typedef struct _ModmanGUIObject {
	GtkButton * load ;
	GtkButton * unload ;
	GtkButton * load1 ;
	GtkTreeSelection * selection ;
	GtkCheckButton * check ;
} ModmanGUIObject ;

void modman_gui ( void ) ;
gchar * modman_gui_get_selected ( GtkTreeView * view ) ;

void modman_gui_load_module ( GtkWidget * widget, shagpointer data ) ;
void modman_gui_unload_module ( GtkWidget * widget, shagpointer data ) ;
void modman_gui_module_info ( GtkWidget * widget, shagpointer data )  ;
void susie_message_box_detailed_info ( gchar * header, gchar * text ) ;
void modman_gui_load_module_with_arguments ( GtkWidget * widget, shagpointer data ) ;
void modman_gui_update_buttons ( GtkTreeSelection * selection, ModmanGUIObject * object ) ;
gboolean modman_gui_can_modules_be_automatically_loaded_question_mark ( void ) ;
void modman_gui_set_load_automatically ( GtkWidget * widget, shagpointer data ) ;
gchar * modman_gui_get_selected_name ( GtkTreeView * view ) ;
