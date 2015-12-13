/*
 * Copyright (C) 2015 Andrea Zagli <azagli@libero.it>
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

#include <stdlib.h>
#include <math.h>

#include "generic.h"

int
main (int argc, char *argv[])
{
	gchar *str;
	guint n_dec;
	gboolean with_currency_symbol;

	n_dec = -1;
	with_currency_symbol = FALSE;

	if (argc >= 3)
		{
			n_dec = strtol (argv[2], NULL, 10);
		}
	if (argc >= 4)
		{
		    with_currency_symbol = TRUE;
		}

	g_message ("VALUE %s DOUBLE %s", argv[1], g_strdup_printf (g_strdup_printf ("%%.%df", n_dec), g_strtod (argv[1], NULL)));
	g_message ("DECIMALS %d", n_dec);
	str = zak_utils_format_money (g_strtod (argv[1], NULL), n_dec, with_currency_symbol);
	g_message ("FORMAT %s", str);

	g_message ("UNFORMAT %f", zak_utils_unformat_money (str));

	return 0;
}
