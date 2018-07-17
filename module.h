/*
 *      module.h
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

void modman_module_load ( gchar * module ) ;
void modman_module_unload ( gchar * module ) ;
gchar * modman_module_get_info ( gchar * module ) ;
gchar * modman_module_remove_extension ( gchar * module ) ;

gchar * modman_module_get_kernel_version ( void ) ;
gboolean modman_module_get_loaded ( gchar * module ) ;
gboolean modman_module_get_loaded_automatically ( gchar * module ) ;

gchar * modman_module_get_description ( gchar * module ) ;
