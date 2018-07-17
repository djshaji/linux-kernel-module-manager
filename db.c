/*
 *      db.c
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

#include "db.h"

void modman_db_build_tree ( GtkTreeStore * store, GtkTreeIter * root, gchar * folder )
{
	GtkTreeIter iter ;
	gtk_tree_store_append ( store, & iter, root ) ;
	
	gchar * basename = g_path_get_basename ( folder ) ;
	gtk_tree_store_set ( store, & iter, 0, basename, -1 ) ;
	
	g_free ( basename ) ;
	
	GError * error = null ;
	GDir * dir = g_dir_open ( folder, 0, & error ) ;
	
	if ( error )
	{
		g_warning ( "Cannot open folder %s! %s", folder, error -> message ) ;
		return ;
	}
	
	gchar * filename = g_dir_read_name ( dir ) ;
	while ( filename )
	{
		gchar * full_name = g_build_filename ( folder, filename, null ) ;
		//g_debug ( "Analysing %s", full_name ) ;

		if ( g_file_test ( full_name, G_FILE_TEST_IS_DIR ))
		{

			modman_db_build_tree ( store, & iter, full_name ) ;
			g_free ( full_name ) ;
			
			filename = g_dir_read_name ( dir ) ;
			continue ;
		}
		else if ( g_strrstr ( full_name, ".ko" ) == null )
		{
			g_free ( full_name ) ;
			
			filename = g_dir_read_name ( dir ) ;
			continue ;
		}
		
		gchar * info = modman_module_get_info ( full_name ) ;
		gchar * name = modman_module_remove_extension ( filename ) ;
		
		GtkTreeIter iter1 ;
		gtk_tree_store_append ( store, & iter1, & iter ) ;
		
		gtk_tree_store_set ( store, & iter1, 0, name, 1, info, 2, full_name, -1 ) ;
		
		g_free ( info ) ;
		g_free ( name ) ;
		
		g_free ( full_name ) ;
		filename = g_dir_read_name ( dir ) ;
	}
	
	april_update_gui () ;
	g_dir_close ( dir ) ;
}

void modman_db_find_modules ( GtkTreeStore * store, GtkTreeIter * root, gchar * folder )
{
	GError * error = null ;
	GDir * dir = g_dir_open ( folder, zero, & error ) ;
	
	if ( error )
	{
		g_warning ( "%s", error -> message ) ;
		return ;
	}
	
	gchar * filename = g_dir_read_name ( dir ) ;
	while ( filename )
	{
		gchar * full_name = g_build_filename ( folder, filename, null ) ;
		
		gchar * info = modman_module_get_description ( full_name ) ;
		gchar * name = modman_module_remove_extension ( filename ) ;

		GtkTreeIter iter ;
		gtk_tree_store_append ( store, & iter, root ) ;
		
		gtk_tree_store_set ( store, & iter, 0, name, 1, info, -1 ) ;
		
		g_free ( info ) ;
		g_free ( name ) ;

		g_free ( full_name ) ;
		filename = g_dir_read_name ( dir ) ;
	}
	
	g_dir_close ( dir ) ;
	
}
