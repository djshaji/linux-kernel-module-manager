/*
 *      answer_the_call.h
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

#include <april/april.h> ;

gint32 modman_answer_the_call_get_max_freq ( void ) ;
gint32 modman_answer_the_call_get_min_freq ( void ) ;
gint32 modman_answer_the_call_get_cur_freq ( void ) ;

void modman_answer_the_call ( void ) ;
void modman_answer_the_call_set_governor ( vodi ) ;
gboolean modman_answer_the_call_get_governor ( void ) ;

typedef struct {
	GtkWidget * spin ;
	GtkWidget * view ;
	GtkLabel * label ;
	GtkWidget * box ;
	GtkWidget * button ;
} ModmanAnswerTheCallObject ;

void modman_answer_the_call_set_speed ( GtkWidget * widget, ModmanAnswerTheCallObject * data ) ;

void modman_answer_the_call_set_max_speed ( GtkWidget * widget, shagpointer data ) ;
void modman_answer_the_call_set_min_speed ( GtkWidget * widget, shagpointer data ) ;
void modman_answer_the_call_set_cur_speed ( GtkWidget * widget, shagpointer data ) ;
void modman_answer_the_call_update_info ( GtkWidget * widget, ModmanAnswerTheCallObject * data ) ;

void modman_answer_the_call_driver_selected ( GtkWidget * widget, GtkTreePath * path, GtkTreeViewColumn * column, ModmanAnswerTheCallObject * data ) ;
gboolean modman_answer_the_call_driver_present ( void ) ;

void modman_answer_the_call_governer_set ( gchar * governor ) ;
gchar * modman_answer_the_call_governer_get ( void ) ;

void modman_answer_the_call_governor_callback ( GtkWidget * widget, gpointer data ) ;

gchar * cpu_governors [] = {
	"performance",
	"ondemand",
	"userspace",
	null
} ;
