/*
 *      module.c
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

#include "module.h"

void modman_module_load ( gchar * module )
{
	gchar * command = g_strdup_printf ( "modprobe -v %s", module ) ;
	april_spawn_async ( command ) ;
	
	do
	{
		gint i = 0 ;
		for ( i = 0 ; i < 10 ; i ++ )
		{
			april_update_gui () ;
			g_usleep ( 100000 ) ;
		}			

	}
	while ( april_check_bin_status ( "modprobe" )) ;
	
	g_free ( command ) ;
}

void modman_module_unload ( gchar * module )
{
	gchar * command = g_strdup_printf ( "modprobe -v -r %s", module ) ;
	april_spawn_async ( command ) ;
	
	do
	{
		gint i = 0 ;
		for ( i = 0 ; i < 10 ; i ++ )
		{
			april_update_gui () ;
			g_usleep ( 100000 ) ;
		}			

	}
	while ( april_check_bin_status ( "modprobe" )) ;
	
	g_free ( command ) ;
}

gchar * modman_module_get_info ( gchar * module )
{
	gchar * command = g_strdup_printf ( "modinfo %s > /tmp/susie.modinfo", module ) ;
	system ( command ) ;
	
	gchar * info = april_file_get_contents ( "/tmp/susie.modinfo" ) ;
	
	g_free ( command ) ;
	return info ;
}

gchar * modman_module_remove_extension ( gchar * module )
{
	gchar * name = strndup ( module, strlen ( module ) - strlen ( g_strrstr ( module, "." ))) ;
	return name ;
}

gchar * modman_module_get_kernel_version ( void )
{
	system ( "uname -r > /tmp/susie.uname" ) ;
	gchar * ver = april_file_get_contents ( "/tmp/susie.uname" ) ;
	
	gchar ** vector = g_strsplit ( ver, "\n", -1 ) ;
	gchar * version = g_strdup ( vector [0] ) ;
	
	g_strfreev ( vector ) ;
	g_free ( ver ) ;
	
	
	return version ;
}

gboolean modman_module_get_loaded ( gchar * module )
{
	gchar * text = april_file_get_contents ( "/proc/modules" ) ;
	gboolean result = g_strrstr ( text, module ) ;
	
	if ( ! result )
	{
		result = g_strrstr ( text, g_strdelimit ( module, "-", '_' )) ;
	}
	
	g_free ( text ) ;
	return result ;
}

gboolean modman_module_get_loaded_automatically ( gchar * module )
{
	gchar * text = april_file_get_contents ( "/usr/etc/modules.conf" ) ;
	gboolean result = g_strrstr ( text, module ) ;
	
	if ( ! result )
	{
		result = g_strrstr ( text, g_strdelimit ( module, "-", '_' )) ;
	}
	
	g_free ( text ) ;
	return result ;
}

gchar * modman_module_get_description ( gchar * module )
{
	gchar * info = modman_module_get_info ( module ) ;
	gchar ** vector = g_strsplit ( info, "\n", -1 ) ;
	
	gint i = 0 ;
	gchar * desc = null ;
	
	while ( vector [i] != null )
	{
		if ( g_strrstr ( vector [i], "description" ) != null )
		{
			gchar ** hector = g_strsplit ( vector [i], ":    ", -1 ) ;
			desc = g_strdup ( hector [1] ) ;
			
			g_strfreev ( hector ) ;
			break ;
		}
		
		i ++ ;
	}
	
	g_free ( info ) ;
	g_strfreev ( vector ) ;
	
	return desc ;
}

