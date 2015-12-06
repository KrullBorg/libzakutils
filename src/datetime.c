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

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <stdlib.h>

#include "generic.h"
#include "datetime.h"


/**
 * zak_utils_get_now_tm:
 *
 */
struct tm
*zak_utils_get_now_tm (void)
{
	time_t tt;

	tt = time (NULL);

	return localtime (&tt);
}

/**
 * zak_utils_get_today_gdate:
 *
 */
GDate
*zak_utils_get_today_gdate (void)
{
	struct tm *tt_tm = zak_utils_get_now_tm ();

	return g_date_new_dmy ((GDateDay)tt_tm->tm_mday,
	                       (GDateMonth)tt_tm->tm_mon + 1,
	                       (GDateYear)tt_tm->tm_year + 1900);
}

/**
 * zak_utils_tm_to_gdatetime:
 * @tm_date:
 *
 */
GDateTime
*zak_utils_tm_to_gdatetime (struct tm *tm_date)
{
	GDateTime *ret;

	ret = NULL;
	if (tm_date != NULL)
		{
			ret = g_date_time_new_local (tm_date->tm_year + 1900,
			                             tm_date->tm_mon + 1,
			                             tm_date->tm_mday,
			                             tm_date->tm_hour,
			                             tm_date->tm_min,
			                             tm_date->tm_sec);
		}

	return ret;
}


/**
 * zak_utils_tm_to_gdate:
 * @tm_date:
 *
 * Returns: a #GDate that represent the struct tm without time information.
 */
GDate
*zak_utils_tm_to_gdate (struct tm *tm_date)
{
	GDate *ret;

	ret = NULL;

	if (tm_date != NULL)
		{
			ret = g_date_new_dmy (tm_date->tm_mday,
			                      tm_date->tm_mon + 1,
			                      tm_date->tm_year + 1900);

			if (ret == NULL || !g_date_valid (ret))
				{
					ret = NULL;
				}
		}

	return ret;
}

/**
 * zak_utils_tm_to_gdate:
 * @gdate:
 *
 */
struct tm
*zak_utils_gdate_to_tm (GDate *gdate)
{
	return zak_utils_gdatetime_to_tm (zak_utils_gdate_to_gdatetime (gdate));
}

/**
 * zak_utils_gdatetime_to_tm:
 * @gdatetime:
 *
 */
struct tm
*zak_utils_gdatetime_to_tm (GDateTime *gdatetime)
{
	struct tm *ret;

	ret = g_new0 (struct tm, 1);
	ret->tm_year = g_date_time_get_year (gdatetime) - 1900;
	ret->tm_mon = g_date_time_get_month (gdatetime) - 1;
	ret->tm_mday = g_date_time_get_day_of_month (gdatetime);
	ret->tm_hour = g_date_time_get_hour (gdatetime);
	ret->tm_min = g_date_time_get_minute (gdatetime);
	ret->tm_sec = g_date_time_get_second (gdatetime);
	mktime (ret);

	return ret;
}

/**
 * zak_utils_gdate_to_gdatetime:
 * @gdate:
 *
 */
GDateTime
*zak_utils_gdate_to_gdatetime (GDate *gdate)
{
	GDateTime *ret;

	ret = NULL;
	if (gdate != NULL && g_date_valid (gdate))
		{
			ret = g_date_time_new_local (g_date_get_year (gdate),
			                             g_date_get_month (gdate),
			                             g_date_get_day (gdate),
			                             0, 0, 0.0);
		}

	return ret;
}

/**
 * zak_utils_gdatetime_to_gdate:
 * @gdatetime:
 *
 */
GDate
*zak_utils_gdatetime_to_gdate (GDateTime *gdatetime)
{
	GDate *ret;

	ret = NULL;
	if (gdatetime != NULL)
		{
			ret = g_new0 (GDate, 1);
			g_date_set_year (ret, g_date_time_get_year (gdatetime));
			g_date_set_month (ret, g_date_time_get_month (gdatetime));
			g_date_set_day (ret, g_date_time_get_day_of_month (gdatetime));
		}

	return ret;
}

/**
 * zak_utils_tm_to_sql:
 * @tm_data:
 * @format:
 *
 */
const gchar
*zak_utils_tm_to_sql (struct tm *tm_data, const gchar *format)
{
	const gchar *ret;

	ret = g_strdup ("NULL");

	if (tm_data != NULL)
		{
			ret = zak_utils_gdatetime_to_sql (zak_utils_tm_to_gdatetime (tm_data), format);
		}

	return ret;
}

/**
 * zak_utils_gdate_to_sql:
 * @gdate:
 * @format:
 *
 */
const gchar
*zak_utils_gdate_to_sql (GDate *gdate, const gchar *format)
{
	const gchar *ret;

	ret = g_strdup ("NULL");

	if (gdate != NULL && g_date_valid (gdate))
		{
			ret = zak_utils_gdatetime_to_sql (zak_utils_gdate_to_gdatetime (gdate), format == NULL ? "date" : format);
		}

	return ret;
}

/**
 * zak_utils_gdatetime_to_sql:
 * @gdatetime:
 * @format:
 *
 */
const gchar
*zak_utils_gdatetime_to_sql (GDateTime *gdatetime, const gchar *format)
{
	const gchar *ret;
	gchar *_format;

	ret = g_strdup ("NULL");

	if (gdatetime != NULL)
		{
			if (format == NULL)
				{
					_format = g_strdup ("%F %R:%S");
				}
			else
				{
					_format = g_strstrip (g_strdup (format));
					if (g_strcmp0 (_format, "") == 0
					    || g_strcmp0 (_format, "datetime") == 0)
						{
							_format = g_strdup ("%F %R:%S");
						}
					else if (g_strcmp0 (_format, "date") == 0)
						{
							_format = g_strdup ("%F");
						}
					else if (g_strcmp0 (_format, "time") == 0)
						{
							_format = g_strdup ("%R:%S");
						}
				}
			ret = g_strdup_printf ("'%s'", g_date_time_format (gdatetime, _format));
		}

	return ret;
}

/**
 * zak_utils_get_tm_from_string:
 * @string: the #gchar to be parsed.
 * @format:
 *
 */
struct tm
*zak_utils_get_tm_from_string (const gchar *string, const gchar *format)
{
	struct tm *ret;

	GDateTime *gdatetime;

	ret = NULL;

	gdatetime = zak_utils_get_gdatetime_from_string (string, format);

	if (gdatetime != NULL)
		{
			ret = g_malloc0 (sizeof (struct tm));
			ret->tm_year = g_date_time_get_year (gdatetime) - 1900;
			ret->tm_mon = g_date_time_get_month (gdatetime) - 1;
			ret->tm_mday = g_date_time_get_day_of_month (gdatetime);
			ret->tm_hour = g_date_time_get_hour (gdatetime);
			ret->tm_min = g_date_time_get_minute (gdatetime);
			ret->tm_sec = g_date_time_get_second (gdatetime);
			mktime (ret);

			g_object_unref (gdatetime);
		}

	return ret;
}

/**
 * zak_utils_get_gdate_from_string:
 * @string:
 * @format:
 *
 */
GDate
*zak_utils_get_gdate_from_string (const gchar *string, const gchar *format)
{
	GDate *ret;

	GDateTime *gdatetime;

	gdatetime = zak_utils_get_gdatetime_from_string (string, format);

	ret = zak_utils_gdatetime_to_gdate (gdatetime);

	g_object_unref (gdatetime);

	return ret;
}

/**
 * zak_utils_get_gdatetime_from_string:
 * @string:
 * @format:
 *
 */
GDateTime
*zak_utils_get_gdatetime_from_string (const gchar *string, const gchar *format)
{
	GDateTime *ret;

	gchar *new_str;
	gchar *new_format;
	gchar **str_tokens;
	gchar **format_tokens;
	gchar *delimiters;
	guint len_strv;
	guint len_formatv;
	guint i;
	guint i_to;

	gint year;
	gint month;
	gint day;
	gint hour;
	gint minute;
	gdouble seconds;

	gboolean error;

	g_return_val_if_fail (string != NULL, NULL);

	new_str = g_strstrip (g_strdup (string));
	if (g_strcmp0 (new_str, "") == 0)
		{
			g_free (new_str);
			return NULL;
		}

	if (format == NULL)
		{
			new_format = g_strdup ("%Y-%m-%d %H:%M:%S");
		}
	else
		{
			new_format = g_strstrip (g_strdup (format));
			if (g_strcmp0 (new_format, "") == 0)
				{
					new_format = g_strdup ("%Y-%m-%d %H:%M:%S");
				}
		}

	/* removes format identifiers to find delimiters */
	delimiters = zak_utils_string_replace (new_format, "%Y", "");
	delimiters = zak_utils_string_replace (delimiters, "%m", "");
	delimiters = zak_utils_string_replace (delimiters, "%d", "");
	delimiters = zak_utils_string_replace (delimiters, "%H", "");
	delimiters = zak_utils_string_replace (delimiters, "%M", "");
	delimiters = zak_utils_string_replace (delimiters, "%S", "");

	if (delimiters == NULL || g_strcmp0 (delimiters, "") == 0)
		{
			g_free (delimiters);
			g_free (new_format);
			g_free (new_str);
			return NULL;
		}

	str_tokens = g_strsplit_set (string, delimiters, -1);
	if (str_tokens == NULL)
		{
			g_free (delimiters);
			g_free (new_format);
			g_free (new_str);
			return NULL;
		}

	format_tokens = g_strsplit_set (format, delimiters, -1);
	if (format_tokens == NULL)
		{
			g_free (delimiters);
			g_free (new_format);
			g_free (new_str);
			return NULL;
		}

	len_strv = g_strv_length (str_tokens);
	len_formatv = g_strv_length (format_tokens);

	year = 1;
	month = 1;
	day = 1;
	hour = 0;
	minute = 0;
	seconds = 0.0;

	error = FALSE;
	ret = NULL;

	i_to = MIN (len_strv, len_formatv);
	for (i = 0; i < i_to; i++)
		{
			if (g_strcmp0 (format_tokens[i], "%Y") == 0)
				{
					year = strtol (str_tokens[i], NULL, 10);
					if (year < 1 || year > 9999)
						{
							error = TRUE;
							break;
						}
				}
			else if (g_strcmp0 (format_tokens[i], "%m") == 0)
				{
					month = strtol (str_tokens[i], NULL, 10);
					if (month < 1 || month > 12)
						{
							error = TRUE;
							break;
						}
				}
			else if (g_strcmp0 (format_tokens[i], "%d") == 0)
				{
					day = strtol (str_tokens[i], NULL, 10);
					if (day < 1 || day > 31)
						{
							error = TRUE;
							break;
						}
				}
			else if (g_strcmp0 (format_tokens[i], "%H") == 0)
				{
					hour = strtol (str_tokens[i], NULL, 10);
					if (hour > 23)
						{
							error = TRUE;
							break;
						}
				}
			else if (g_strcmp0 (format_tokens[i], "%M") == 0)
				{
					minute = strtol (str_tokens[i], NULL, 10);
					if (minute > 59)
						{
							error = TRUE;
							break;
						}
				}
			else if (g_strcmp0 (format_tokens[i], "%S") == 0)
				{
					seconds = g_strtod (str_tokens[i], NULL);
					if (seconds > 59.0)
						{
							error = TRUE;
							break;
						}
				}
		}

	g_strfreev (format_tokens);
	g_strfreev (str_tokens);
	g_free (delimiters);
	g_free (new_format);
	g_free (new_str);

	if (!error)
		{
			ret = g_date_time_new_local (year,
			                             month,
			                             day,
			                             hour,
			                             minute,
			                             seconds);
		}

	return ret;
}

/**
 * zak_utils_tm_format:
 * @tm_date: a tm struct.
 * @format:
 *
 * Returns: a string representation of @tm_date based on the format in @format.
 * It interprets a very little subset of format identifiers from strftime.
 * %Y: the year with 4 digits.
 * %m: the month with 2 digits.
 * %d: the day with 2 digits.
 * %H: the hours with 2 digits 00-24.
 * %M: the minutes with 2 digits 00-59.
 * %S: the seconds with 2 digits 00-59.
 */
gchar
*zak_utils_tm_format (struct tm *tm_date,
					  const gchar *format)
{
	gchar *ret;

	ret = g_strdup ("");

	g_return_val_if_fail (tm_date != NULL, ret);

	ret = zak_utils_string_replace (format, "%Y",
									g_strdup_printf ("%04u", tm_date->tm_year + 1900));
	ret = zak_utils_string_replace (ret, "%m",
									g_strdup_printf ("%02u", tm_date->tm_mon + 1));
	ret = zak_utils_string_replace (ret, "%d",
									g_strdup_printf ("%02u", tm_date->tm_mday));
	ret = zak_utils_string_replace (ret, "%H",
									g_strdup_printf ("%02u", tm_date->tm_hour));
	ret = zak_utils_string_replace (ret, "%M",
									g_strdup_printf ("%02u", tm_date->tm_min));
	ret = zak_utils_string_replace (ret, "%S",
									g_strdup_printf ("%02u", tm_date->tm_sec));

	return ret;
}

/**
 * zak_utils_gate_format:
 * @gdate:
 * @format:
 *
 */
gchar
*zak_utils_gdate_format (GDate *gdate, const gchar *format)
{
	gchar *ret;

	if (g_date_valid (gdate)) return "";

	ret = zak_utils_gdatetime_format (zak_utils_gdate_to_gdatetime (gdate), format);

	return ret;
}

/**
 * zak_utils_gdatetime_format:
 * @gdate:
 * @format:
 *
 */
gchar
*zak_utils_gdatetime_format (GDateTime *gdatetime, const gchar *format)
{
	gchar *ret;

	if (gdatetime == NULL) return "";

	ret = g_date_time_format (gdatetime, format);

	if (ret == NULL) return "";

	return ret;
}
