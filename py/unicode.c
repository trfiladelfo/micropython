/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>

#include "py/unicode.h"

// attribute flags
#define FL_PRINT (0x01)
#define FL_SPACE (0x02)
#define FL_DIGIT (0x04)
#define FL_ALPHA (0x08)
#define FL_UPPER (0x10)
#define FL_LOWER (0x20)

// shorthand character attributes
#define AT_PR (FL_PRINT)
#define AT_SP (FL_SPACE | FL_PRINT)
#define AT_DI (FL_DIGIT | FL_PRINT)
#define AT_AL (FL_ALPHA | FL_PRINT)
#define AT_UP (FL_UPPER | FL_ALPHA | FL_PRINT)
#define AT_LO (FL_LOWER | FL_ALPHA | FL_PRINT)

// table of attributes for ascii characters
STATIC const uint8_t attr[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, AT_SP, AT_SP, AT_SP, AT_SP, AT_SP, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    AT_SP, AT_PR, AT_PR, AT_PR, AT_PR, AT_PR, AT_PR, AT_PR,
    AT_PR, AT_PR, AT_PR, AT_PR, AT_PR, AT_PR, AT_PR, AT_PR,
    AT_DI, AT_DI, AT_DI, AT_DI, AT_DI, AT_DI, AT_DI, AT_DI,
    AT_DI, AT_DI, AT_PR, AT_PR, AT_PR, AT_PR, AT_PR, AT_PR,
    AT_PR, AT_UP, AT_UP, AT_UP, AT_UP, AT_UP, AT_UP, AT_UP,
    AT_UP, AT_UP, AT_UP, AT_UP, AT_UP, AT_UP, AT_UP, AT_UP,
    AT_UP, AT_UP, AT_UP, AT_UP, AT_UP, AT_UP, AT_UP, AT_UP,
    AT_UP, AT_UP, AT_UP, AT_PR, AT_PR, AT_PR, AT_PR, AT_PR,
    AT_PR, AT_LO, AT_LO, AT_LO, AT_LO, AT_LO, AT_LO, AT_LO,
    AT_LO, AT_LO, AT_LO, AT_LO, AT_LO, AT_LO, AT_LO, AT_LO,
    AT_LO, AT_LO, AT_LO, AT_LO, AT_LO, AT_LO, AT_LO, AT_LO,
    AT_LO, AT_LO, AT_LO, AT_PR, AT_PR, AT_PR, AT_PR, 0
};

// TODO: Rename to str_get_char
unichar utf8_get_char(const byte *s) {
#if MICROPY_PY_BUILTINS_STR_UNICODE
    unichar ord = *s++;
    if (!UTF8_IS_NONASCII(ord)) return ord;
    ord &= 0x7F;
    for (unichar mask = 0x40; ord & mask; mask >>= 1) {
        ord &= ~mask;
    }
    while (UTF8_IS_CONT(*s)) {
        ord = (ord << 6) | (*s++ & 0x3F);
    }
    return ord;
#else
    return *s;
#endif
}

// TODO: Rename to str_next_char
const byte *utf8_next_char(const byte *s) {
#if MICROPY_PY_BUILTINS_STR_UNICODE
    ++s;
    while (UTF8_IS_CONT(*s)) {
        ++s;
    }
    return s;
#else
    return s + 1;
#endif
}

mp_uint_t utf8_ptr_to_index(const byte *s, const byte *ptr) {
    mp_uint_t i = 0;
    while (ptr > s) {
        if (!UTF8_IS_CONT(*--ptr)) {
            i++;
        }
    }

    return i;
}

// TODO: Rename to str_charlen
mp_uint_t unichar_charlen(const char *str, mp_uint_t len)
{
#if MICROPY_PY_BUILTINS_STR_UNICODE
    mp_uint_t charlen = 0;
    for (const char *top = str + len; str < top; ++str) {
        if (!UTF8_IS_CONT(*str)) {
            ++charlen;
        }
    }
    return charlen;
#else
    return len;
#endif
}

// Be aware: These unichar_is* functions are actually ASCII-only!
bool unichar_isspace(unichar c) {
    return c < 128 && (attr[c] & FL_SPACE) != 0;
}

bool unichar_isalpha(unichar c) {
    return c < 128 && (attr[c] & FL_ALPHA) != 0;
}

bool unichar_isprint(unichar c) {
    return c < 128 && (attr[c] & FL_PRINT) != 0;
}

bool unichar_isdigit(unichar c) {
    return c < 128 && (attr[c] & FL_DIGIT) != 0;
}

bool unichar_isxdigit(unichar c) {
    return unichar_isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

/*
bool unichar_is_alpha_or_digit(unichar c) {
    return c < 128 && (attr[c] & (FL_ALPHA | FL_DIGIT)) != 0;
}
*/

bool unichar_isupper(unichar c) {
    return c < 128 && (attr[c] & FL_UPPER) != 0;
}

bool unichar_islower(unichar c) {
    return c < 128 && (attr[c] & FL_LOWER) != 0;
}

unichar unichar_tolower(unichar c) {
    if (unichar_isupper(c)) {
        return c + 0x20;
    }
    return c;
}

unichar unichar_toupper(unichar c) {
    if (unichar_islower(c)) {
        return c - 0x20;
    }
    return c;
}
