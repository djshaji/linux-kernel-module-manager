/*
 *      loader.c
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
#include <string.h>

void load_modules ( void ) ;
void load_module ( gchar * filename ) ;

gint main ( gint argc, gchar ** argv )
{
	g_set_application_name ( "April module loader 1.0" ) ;
	
	load_modules () ;
	return 0;
}

void load_module ( gchar * filename )
{
	gchar * command = g_strdup_printf ( "modprobe %s", filename ) ;
	system ( command ) ;
	
	g_free ( command ) ;
}

void load_modules ( void )
{
	GError * error = null ;
	gchar * contents = null ;
	
	g_file_get_contents ( "/usr/etc/modules.conf", & contents, null, & error ) ;
	if ( error )
	{
		g_critical ( "April module loader: Cannot read configuration file - %s", error -> message ) ;
		return ;
	}
	
	gchar ** vector = g_strsplit ( contents, "\n", -1 ) ;
	gint i = 0 ;
	
	while ( vector [i] != null )
	{
		// Empty line
		if ( strlen ( vector [i] ) == 0 )
		{
			i ++ ;
			continue ;
		}
		
		if ( vector [i][0] != '#' )
			load_module ( vector [i] ) ;
		
		g_debug ( "Loaded module %s", vector [i] ) ;
		i ++ ;
	}
	
	g_strfreev ( vector ) ;
	g_free ( contents ) ;
}
