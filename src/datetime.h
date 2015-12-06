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

#ifndef __ZAK_UTILS_DATETIME_H__
#define __ZAK_UTILS_DATETIME_H__


#include <glib.h>
#include <glib-object.h>


G_BEGIN_DECLS


struct tm *zak_utils_get_now_tm (void);
GDate *zak_utils_get_today_gdate (void);

GDateTime *zak_utils_tm_to_gdatetime (struct tm *tm_date);
struct tm *zak_utils_gdatetime_to_tm (GDateTime *gdatetime);

GDateTime *zak_utils_gdate_to_gdatetime (GDate *gdate);
GDate *zak_utils_gdatetime_to_gdate (GDateTime *gdatetime);

const gchar *zak_utils_tm_to_sql (struct tm *tm_date, const gchar *format);
const gchar *zak_utils_gdate_to_sql (GDate *gdate, const gchar *format);
const gchar *zak_utils_gdatetime_to_sql (GDateTime *gdatetime, const gchar *format);

GDate *zak_utils_get_gdate_from_string (const gchar *string, const gchar *format);
GDateTime *zak_utils_get_gdatetime_from_string (const gchar *string, const gchar *format);
gchar *zak_utils_gdate_format (GDate *gdate, const gchar *format);
gchar *zak_utils_gdatetime_format (GDateTime *gdatetime, const gchar *format);


G_END_DECLS


#endif /* __ZAK_UTILS_DATETIME_H__ */
