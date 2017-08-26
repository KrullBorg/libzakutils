/*
 * Copyright (C) 2016 Andrea Zagli <azagli@libero.it>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "datetime.h"

int
main (int argc, char *argv[])
{
	gchar *str;
	gchar *format;

	str = argv[1];

	if (argc > 2)
		{
			format = argv[2];
		}
	else
		{
			format = NULL;
		}

	g_message ("DATE TIME: %s", zak_utils_gdatetime_format (zak_utils_get_gdatetime_from_string (str, format), "%F %T"));

	return 0;
}
