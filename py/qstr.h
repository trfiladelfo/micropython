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
#ifndef __MICROPY_INCLUDED_PY_QSTR_H__
#define __MICROPY_INCLUDED_PY_QSTR_H__

#include "py/mpconfig.h"
#include "py/misc.h"

// See qstrdefs.h for a list of qstr's that are available as constants.
// Reference them as MP_QSTR_xxxx.
//
// Note: it would be possible to define MP_QSTR_xxx as qstr_from_str_static("xxx")
// for qstrs that are referenced this way, but you don't want to have them in ROM.

// first entry in enum will be MP_QSTR_NULL=0, which indicates invalid/no qstr
enum {
#define QDEF(id, str) id,
#include "genhdr/qstrdefs.generated.h"
#undef QDEF
    MP_QSTR_number_of,
};

typedef mp_uint_t qstr;

typedef struct _qstr_pool_t {
    struct _qstr_pool_t *prev;
    mp_uint_t total_prev_len;
    mp_uint_t alloc;
    mp_uint_t len;
    const byte *qstrs[];
} qstr_pool_t;

#define QSTR_FROM_STR_STATIC(s) (qstr_from_strn((s), strlen(s)))

void qstr_init(void);

mp_uint_t qstr_compute_hash(const byte *data, mp_uint_t len);
qstr qstr_find_strn(const char *str, mp_uint_t str_len); // returns MP_QSTR_NULL if not found

qstr qstr_from_str(const char *str);
qstr qstr_from_strn(const char *str, mp_uint_t len);

byte* qstr_build_start(mp_uint_t len, byte **q_ptr);
qstr qstr_build_end(byte *q_ptr);

mp_uint_t qstr_hash(qstr q);
const char *qstr_str(qstr q);
mp_uint_t qstr_len(qstr q);
const byte *qstr_data(qstr q, mp_uint_t *len);

void qstr_pool_info(mp_uint_t *n_pool, mp_uint_t *n_qstr, mp_uint_t *n_str_data_bytes, mp_uint_t *n_total_bytes);
void qstr_dump_data(void);

#endif // __MICROPY_INCLUDED_PY_QSTR_H__
