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

#ifndef __ZAK_UTILS_H__
#define __ZAK_UTILS_H__


#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>


G_BEGIN_DECLS


gboolean zak_utils_file_exists (const gchar *filename);

gdouble zak_utils_round (gdouble value, guint n_decimals);

gchar *zak_utils_format_money (gdouble number, gint decimals, gboolean with_currency_symbol);
gdouble zak_utils_unformat_money (const gchar *value);

void zak_utils_gstring_initial_capital (GString *gstring);

int zak_utils_compare_version (const gchar *ver1, const gchar *ver2, const gchar *delimiters, guint *part);

GValue *zak_utils_gvalue_new_int (int i);
GValue *zak_utils_gvalue_new_string (const gchar *str);
GValue *zak_utils_gvalue_new_boolean (gboolean b);
GValue *zak_utils_gvalue_new_float (gfloat f);

gboolean zak_utils_str_to_boolean (const gchar *str);


G_END_DECLS


#endif /* __ZAK_UTILS_H__ */
