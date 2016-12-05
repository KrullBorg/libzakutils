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
#include <math.h>
#include <string.h>
#include <locale.h>

#include "generic.h"
#include "datetime.h"


/**
 * zak_utils_file_exists:
 * @filename:
 *
 */
gboolean
zak_utils_file_exists (const gchar *filename)
{
	gboolean ret;

	GFile *f = g_file_new_for_path (filename);

	ret = g_file_query_exists (f, NULL);

	g_object_unref (f);

	return ret;
}

/**
 * zak_utils_string_replace:
 * @string:
 * @origin:
 * @replace:
 *
 */
gchar
*zak_utils_string_replace (const gchar *string,
						   const gchar *origin,
						   const gchar *replace)
{
	gchar *ret;
	gchar *p;

	if (string == NULL) return NULL;
	if (origin == NULL || replace == NULL) return g_strdup (string);

	p = g_strstr_len (string, -1, origin);

	if (p == NULL)
		{
			return g_strdup (string);
		}

	ret = g_strndup (string, p - string);

	ret = g_strdup_printf ("%s%s%s", ret, replace, p + strlen (origin));

	return ret;
}

/**
 * zak_utils_round:
 * @value:
 * @n_decimals:
 *
 */
gdouble
zak_utils_round (gdouble value, guint n_decimals)
{
	gfloat elev;
	gint i;
	gfloat ret;

	elev = pow (10.0, (gfloat)n_decimals);

	ret = (value * elev) + 0.5;
	i = (gint)ret;

	ret = (gfloat)i / elev;

	return ret;
}

/**
 * zak_utils_format_money_full:
 * @number:
 * @decimals:
 * @thousands_separator:
 * @currency_symbol:
 *
 */
gchar
*zak_utils_format_money_full (gdouble number,
							  gint decimals,
							  const gchar *thousands_separator,
							  const gchar *currency_symbol)
{
	gchar *ret;

	GRegex *regex;
	GError *error;

	gchar *str_format;
	gchar *str;
	gssize str_len;

	/* TODO
	 * - get number of decimals from locale
	 */
	struct lconv *localeinfo;

	setlocale (LC_NUMERIC, "");
	setlocale (LC_MONETARY, "");

	localeinfo = localeconv ();

	ret = g_strdup ("");

	error = NULL;
	regex = g_regex_new ("(^[-\\d]?\\d+)(\\d\\d\\d)", 0, 0, &error);
	if (regex == NULL || error != NULL)
		{
			g_warning ("Error on creating regex: %s.",
			           error->message != NULL ? error->message : "no details");
			return "";
		}

	str_format = g_strdup_printf ("%%0%sf", decimals == 0 ? ".0" : (decimals < 0 ? ".2" : g_strdup_printf (".%d", decimals)));
	ret = g_strdup_printf (str_format, number);

	while (TRUE)
		{
			error = NULL;
			str_len = g_utf8_strlen (ret, -1);
			str = g_regex_replace ((const GRegex *)regex,
			                       ret, str_len, 0,
			                       g_strdup_printf ("\\1%s\\2", thousands_separator == NULL ? localeinfo->mon_thousands_sep : thousands_separator), 0,
			                       &error);
			if (error != NULL)
				{
					g_warning ("Error on regex replacing: %s.",
					           error->message != NULL ? error->message : "no details");
					g_regex_unref (regex);
					return "";
				}
			if (g_strcmp0 (ret, str) != 0)
				{
					ret = g_strdup (str);
					g_free (str);
				}
			else
				{
					break;
				}
		}

	if (currency_symbol != NULL)
		{
			ret = g_strconcat (g_strcmp0 (currency_symbol, "") == 0 ? localeinfo->currency_symbol : currency_symbol, " ", ret, NULL);
		}

	g_regex_unref (regex);

	return ret;
}

/**
 * zak_utils_format_money:
 * @number:
 * @decimals:
 * @with_currency_symbol:
 *
 */
gchar
*zak_utils_format_money (gdouble number,
						 gint decimals,
						 gboolean with_currency_symbol)
{
	return zak_utils_format_money_full (number, decimals, NULL, with_currency_symbol ? "" : NULL);
}

/**
 * zak_utils_unformat_money_full:
 * @value:
 * @thousand_separator:
 * @currency_symbol:
 *
 */
gdouble
zak_utils_unformat_money_full (const gchar *value,
							   const gchar *thousands_separator,
							   const gchar *currency_symbol)
{
	gdouble ret;

	GRegex *regex;
	GError *error;

	gchar *str;

	struct lconv *localeinfo;

	ret = 0.0;

	setlocale (LC_NUMERIC, "");
	setlocale (LC_MONETARY, "");

	localeinfo = localeconv ();

	error = NULL;
	regex = g_regex_new (g_strdup_printf ("[%s %s]",
										  currency_symbol != NULL && g_strcmp0 (currency_symbol, "") != 0 ? currency_symbol : localeinfo->currency_symbol,
										  thousands_separator != NULL ? thousands_separator : localeinfo->mon_thousands_sep), 0, 0, &error);
	if (error != NULL)
		{
			g_warning ("Error on creating regex: %s.",
			           error->message != NULL ? error->message : "no details");
			return ret;
		}

	error = NULL;
	str = g_regex_replace ((const GRegex *)regex,
	                       value, -1, 0,
	                       "", 0,
	                       &error);
	if (error != NULL)
		{
			g_warning ("Error on regex replacing: %s.",
			           error->message != NULL ? error->message : "no details");
			g_regex_unref (regex);
			return ret;
		}

	ret = g_strtod (str, NULL);

	g_regex_unref (regex);

	return ret;
}

/**
 * zak_utils_unformat_money:
 * @value:
 *
 */
gdouble
zak_utils_unformat_money (const gchar *value)
{
	return zak_utils_unformat_money_full (value, NULL, NULL);
}

/**
 * zak_utils_gstring_initial_capital:
 * @gstring:
 *
 */
void
zak_utils_gstring_initial_capital (GString *gstring)
{
	const gchar *cutf8;

	GString *tmp;
	gboolean first;
	gunichar c;

	g_return_if_fail (gstring != NULL);
	g_return_if_fail (gstring->str != NULL);
	g_return_if_fail (g_utf8_validate (gstring->str, -1, NULL));

	tmp = g_string_new ("");
	first = TRUE;

	cutf8 = gstring->str;
	while (*cutf8)
		{
			c = g_utf8_get_char (cutf8);
			if (!g_unichar_isalpha (c))
				{
					first = TRUE;
					g_string_append_unichar (tmp, c);
				}
			else if (first)
				{
					first = FALSE;
					g_string_append_unichar (tmp, g_unichar_toupper (c));
				}
			else
				{
					g_string_append_unichar (tmp, g_unichar_tolower (c));
				}
			cutf8 = g_utf8_next_char (cutf8);
		}

	g_string_assign (gstring, tmp->str);
	g_string_free (tmp, TRUE);
}

/**
 * zak_utils_compare_version:
 * @ver1:
 * @ver2:
 * @delimiters:
 * @part:
 *
 */
int
zak_utils_compare_version (const gchar *ver1, const gchar *ver2, const gchar *delimiters, guint *part)
{
	gchar **_ver1;
	gchar **_ver2;

	gchar *_delimiters;

	guint i;
	guint l;

	if (ver1 == NULL)
		{
			return -1;
		}
	if (ver2 == NULL)
		{
			return 1;
		}

	if (delimiters == NULL)
		{
			_delimiters = g_strdup (".");
		}
	else
		{
			_delimiters = g_strstrip (g_strdup (delimiters));
			if (g_strcmp0 (_delimiters, "") == 0)
				{
					g_free (_delimiters);
					_delimiters = g_strdup (".");
				}
		}

	_ver1 = g_strsplit (g_strstrip (g_strdup (ver1)), _delimiters, -1);
	_ver2 = g_strsplit (g_strstrip (g_strdup (ver2)), _delimiters, -1);

	l = MIN (g_strv_length (_ver1), g_strv_length (_ver2));
	for (i = 0; i < l; i++)
		{
			if (strtol (_ver1[i], NULL, 10) < strtol (_ver2[i], NULL, 10))
				{
					if (part != NULL)
						{
							*part = i;
						}
					return -1;
				}
			else if (strtol (_ver1[i], NULL, 10) > strtol (_ver2[i], NULL, 10))
				{
					if (part != NULL)
						{
							*part = i;
						}
					return 1;
				}
		}

	if (g_strv_length (_ver1) < g_strv_length (_ver2))
		{
			*part = g_strv_length (_ver1) + 1;
			return -1;
		}
	else if (g_strv_length (_ver1) > g_strv_length (_ver2))
		{
			*part = g_strv_length (_ver2) + 1;
			return 1;
		}

	*part = 0;
	return 0;
}

/**
 * zak_utils_gvalue_new_int:
 * @i:
 *
 */
GValue
*zak_utils_gvalue_new_int (int i)
{
	GValue *gval;

	gval = g_new0 (GValue, 1);
	g_value_init (gval, G_TYPE_INT);
	g_value_set_int (gval, i);

	return gval;
}

/**
 * zak_utils_gvalue_new_string:
 * @str:
 *
 */
GValue
*zak_utils_gvalue_new_string (const gchar *str)
{
	GValue *gval;

	gval = g_new0 (GValue, 1);
	g_value_init (gval, G_TYPE_STRING);
	g_value_set_string (gval, str);

	return gval;
}

/**
 * zak_utils_gvalue_new_boolean:
 * @b:
 *
 */
GValue
*zak_utils_gvalue_new_boolean (gboolean b)
{
	GValue *gval;

	gval = g_new0 (GValue, 1);
	g_value_init (gval, G_TYPE_BOOLEAN);
	g_value_set_boolean (gval, b);

	return gval;
}

/**
 * zak_utils_gvalue_new_float:
 * @f:
 *
 */
GValue
*zak_utils_gvalue_new_float (gfloat f)
{
	GValue *gval;

	gval = g_new0 (GValue, 1);
	g_value_init (gval, G_TYPE_FLOAT);
	g_value_set_float (gval, f);

	return gval;
}

/**
 * zak_utils_gvalue_new_gdatetime:
 * @datetime:
 *
 */
GValue
*zak_utils_gvalue_new_gdatetime (GDateTime *datetime)
{
	GValue *gval;

	gval = g_new0 (GValue, 1);
	g_value_init (gval, G_TYPE_DATE_TIME);
	g_value_set_boxed (gval, datetime);

	return gval;
}

/**
 * zak_utils_string_to_boolean:
 * @value:
 *
 * Utility function to convert a string to its #gboolean equivalent.
 *
 * Returns: the #gboolean equivalent from @value.
 */
gboolean
zak_utils_string_to_boolean (const gchar *str)
{
	gchar *str_value;
	gboolean bool_value;

	if (str == NULL)
		{
			return FALSE;
		}

	bool_value = FALSE;

	str_value = g_strstrip (g_strdup (str));

	if (strcmp (str_value, "0") == 0
			|| strcasecmp (str_value, "f") == 0
			|| strcasecmp (str_value, "false") == 0
			|| strcasecmp (str_value, "n") == 0
			|| strcasecmp (str_value, "no") == 0
			|| strcasecmp (str_value, "off") == 0)
		{
			bool_value = FALSE;
		}
	else if (strcmp (str_value, "1") == 0
			|| strcasecmp (str_value, "t") == 0
			|| strcasecmp (str_value, "true") == 0
			|| strcasecmp (str_value, "y") == 0
			|| strcasecmp (str_value, "yes") == 0
			|| strcasecmp (str_value, "on") == 0)
		{
			bool_value = TRUE;
		}

	g_free (str_value);

	return bool_value;
}

/**
 * zak_utils_ghashtable_get_string:
 * @ht:
 * @key:
 *
 * Returns:
 */
gchar
*zak_utils_ghashtable_get_string (GHashTable *ht, gconstpointer key)
{
	gchar *ret;

	GValue *gv;

	gv = g_hash_table_lookup (ht, key);
	if (gv == NULL
		|| !G_VALUE_HOLDS_STRING (gv))
		{
			ret = g_strdup ("");
		}
	else
		{
			ret = g_value_dup_string (gv);
		}

	return ret;
}

/**
 * zak_utils_ghashtable_get_boolean:
 * @ht:
 * @key:
 *
 * Returns:
 */
gboolean
zak_utils_ghashtable_get_boolean (GHashTable *ht, gconstpointer key)
{
	gboolean ret;

	GValue *gv;

	gv = g_hash_table_lookup (ht, key);
	if (gv == NULL
		|| !G_VALUE_HOLDS_BOOLEAN (gv))
		{
			if (G_VALUE_HOLDS_STRING (gv))
				{
					ret = zak_utils_string_to_boolean (g_value_get_string (gv));
				}
			else
				{
					ret = FALSE;
				}
		}
	else
		{
			ret = g_value_get_boolean (gv);
		}

	return ret;
}

/**
 * zak_utils_ghashtable_get_int:
 * @ht:
 * @key:
 *
 * Returns:
 */
gint
zak_utils_ghashtable_get_int (GHashTable *ht, gconstpointer key)
{
	gint ret;

	GValue *gv;

	gv = g_hash_table_lookup (ht, key);
	if (gv == NULL
		|| !G_VALUE_HOLDS_INT (gv))
		{
			ret = 0;
		}
	else
		{
			ret = g_value_get_int (gv);
		}

	return ret;
}

/**
 * zak_utils_ghashtable_get_int_format:
 * @ht:
 * @key:
 * @thousands_separator:
 *
 * Returns:
 */
gchar
*zak_utils_ghashtable_get_int_format (GHashTable *ht, gconstpointer key, const gchar *thousands_separator)
{
	return zak_utils_format_money_full ((gdouble)zak_utils_ghashtable_get_int (ht, key), 0, thousands_separator, NULL);
}

/**
 * zak_utils_ghashtable_get_double:
 * @ht:
 * @key:
 *
 * Returns:
 */
gdouble
zak_utils_ghashtable_get_double (GHashTable *ht, gconstpointer key)
{
	gdouble ret;

	GValue *gv;

	gv = g_hash_table_lookup (ht, key);
	if (gv == NULL
		|| !G_VALUE_HOLDS_DOUBLE (gv))
		{
			ret = 0;
		}
	else
		{
			ret = g_value_get_double (gv);
		}

	return ret;
}

/**
 * zak_utils_ghashtable_get_double_format:
 * @ht:
 * @key:
 * @decimals
 * @thousands_separator:
 * @currency_symbol:
 *
 * Returns:
 */
gchar
*zak_utils_ghashtable_get_double_format (GHashTable *ht, gconstpointer key, gint decimals, const gchar *thousands_separator, const gchar *currency_symbol)
{
	return zak_utils_format_money_full (zak_utils_ghashtable_get_double (ht, key), decimals, thousands_separator, currency_symbol);
}

/**
 * zak_utils_ghashtable_get_float:
 * @ht:
 * @key:
 *
 * Returns:
 */
gfloat
zak_utils_ghashtable_get_float (GHashTable *ht, gconstpointer key)
{
	gfloat ret;

	GValue *gv;

	gv = g_hash_table_lookup (ht, key);
	if (gv == NULL
		|| !G_VALUE_HOLDS_FLOAT (gv))
		{
			ret = 0;
		}
	else
		{
			ret = g_value_get_float (gv);
		}

	return ret;
}

/**
 * zak_utils_ghashtable_get_float_format:
 * @ht:
 * @key:
 * @decimals
 * @thousands_separator:
 * @currency_symbol:
 *
 * Returns:
 */
gchar
*zak_utils_ghashtable_get_float_format (GHashTable *ht, gconstpointer key, gint decimals, const gchar *thousands_separator, const gchar *currency_symbol)
{
	return zak_utils_format_money_full ((gdouble)zak_utils_ghashtable_get_float (ht, key), decimals, thousands_separator, currency_symbol);
}

/**
 * zak_utils_ghashtable_get_gdatetime:
 * @ht:
 * @key:
 *
 * Returns:
 */
GDateTime
*zak_utils_ghashtable_get_gdatetime (GHashTable *ht, gconstpointer key)
{
	GDateTime *ret;

	GValue *gv;

	gv = g_hash_table_lookup (ht, key);
	if (gv == NULL
		|| !G_VALUE_HOLDS_POINTER (gv))
		{
			ret = NULL;
		}
	else
		{
			ret = (GDateTime *)g_value_get_pointer (gv);
		}

	return ret;
}

/**
 * zak_utils_ghashtable_get_gdatetime_format:
 * @ht:
 * @key:
 * @format:
 *
 * Returns:
 */
gchar
*zak_utils_ghashtable_get_gdatetime_format (GHashTable *ht, gconstpointer key, const gchar *format)
{
	gchar *ret;

	GDateTime *gdt;

	gdt = zak_utils_ghashtable_get_gdatetime (ht, key);
	ret = zak_utils_gdatetime_format (gdt, format);

	g_date_time_unref (gdt);

	return ret;
}
