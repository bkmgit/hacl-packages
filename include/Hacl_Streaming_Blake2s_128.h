/* MIT License
 *
 * Copyright (c) 2016-2020 INRIA, CMU and Microsoft Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef __Hacl_Streaming_Blake2s_128_H
#define __Hacl_Streaming_Blake2s_128_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <string.h>
#include "krml/internal/types.h"
#include "krml/lowstar_endianness.h"
#include "krml/internal/target.h"


#include "Hacl_Hash_Blake2s_128.h"

typedef struct Hacl_Streaming_Blake2s_128_blake2s_128_block_state_s
{
  Lib_IntVector_Intrinsics_vec128 *fst;
  Lib_IntVector_Intrinsics_vec128 *snd;
}
Hacl_Streaming_Blake2s_128_blake2s_128_block_state;

typedef struct Hacl_Streaming_Blake2s_128_blake2s_128_state_s
{
  Hacl_Streaming_Blake2s_128_blake2s_128_block_state block_state;
  uint8_t *buf;
  uint64_t total_len;
}
Hacl_Streaming_Blake2s_128_blake2s_128_state;

/**
  State allocation function when there is no key
*/
Hacl_Streaming_Blake2s_128_blake2s_128_state
*Hacl_Streaming_Blake2s_128_blake2s_128_no_key_create_in(void);

/**
  (Re-)initialization function when there is no key
*/
void
Hacl_Streaming_Blake2s_128_blake2s_128_no_key_init(
  Hacl_Streaming_Blake2s_128_blake2s_128_state *s
);

/**
  Update function when there is no key; 0 = success, 1 = max length exceeded
*/
uint32_t
Hacl_Streaming_Blake2s_128_blake2s_128_no_key_update(
  Hacl_Streaming_Blake2s_128_blake2s_128_state *p,
  uint8_t *data,
  uint32_t len
);

/**
  Finish function when there is no key
*/
void
Hacl_Streaming_Blake2s_128_blake2s_128_no_key_finish(
  Hacl_Streaming_Blake2s_128_blake2s_128_state *p,
  uint8_t *dst
);

/**
  Free state function when there is no key
*/
void
Hacl_Streaming_Blake2s_128_blake2s_128_no_key_free(
  Hacl_Streaming_Blake2s_128_blake2s_128_state *s
);

#if defined(__cplusplus)
}
#endif

#define __Hacl_Streaming_Blake2s_128_H_DEFINED
#endif
