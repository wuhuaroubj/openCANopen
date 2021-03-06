/* Copyright (c) 2014-2016, Marel
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "canopen/sdo-dict.h"
#include "string-utils.h"

#include <stdint.h>

#define XSTR(s) STR(s)
#define STR(s) #s

size_t strlcpy(char* dst, const char* src, size_t size);

static const char* str_dict__constname(uint32_t mux)
{
	switch (mux) {
#define X(idx, subidx, type, name) \
	case SDO_MUX(idx, subidx): return XSTR(name);

	SDO_DICTIONARY
#undef X
	default: return "UNKNOWN";
	}
}

enum canopen_type sdo_dict_type(uint32_t mux)
{
	switch (mux) {
#define X(idx, subidx, type, ...) \
	case SDO_MUX(idx, subidx): return CANOPEN_ ## type;

	SDO_DICTIONARY
#undef X
	default: return CANOPEN_UNKNOWN;
	}
}

static inline char* sdo_dict__constname_to_string(char* constname)
{
	return string_tolower(string_replace_char('_', '-', constname));
}

const char* sdo_dict_tostring(uint32_t mux)
{
	static __thread char buffer[64];
	strlcpy(buffer, str_dict__constname(mux), sizeof(buffer));
	return sdo_dict__constname_to_string(buffer);
}

uint32_t sdo_dict_fromstring(const char* str)
{
	static __thread char buffer[64];
	strlcpy(buffer, str, sizeof(buffer));
	string_replace_char('-', '_', buffer);

#define X(idx, subidx, type, name) \
	if (0 == strcasecmp(buffer, XSTR(name))) return SDO_MUX(idx, subidx);

	SDO_DICTIONARY
#undef X

	return 0;
}
