/*
 * Copyright (c) 2017 by Kemu Studio (visit ke.mu)
 *
 * Author(s): Sylwester Wysocki <sw@ke.mu>,
 *            Roman Pietrzak <rp@ke.mu>
 *
 * This file is a part of the KEMU Binary JSON library.
 * See http://bjson.org for more.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _BJSON_DECODE_H_
#define _BJSON_DECODE_H_

#include <stddef.h>
#include <stdint.h>
#include "bjson-common.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Structs and data types.
 */

typedef struct
{
  int (*bjson_null)(void *ctx);
  int (*bjson_boolean)(void *ctx, int value);
  int (*bjson_integer)(void *ctx, long long value);
  int (*bjson_double)(void *ctx, double value);

  int (*bjson_number)(void *ctx, const unsigned char *text, size_t textLen);
  int (*bjson_string)(void *ctx, const unsigned char *text, size_t textLen);

  int (*bjson_start_map)(void *ctx);
  int (*bjson_map_key)(void *ctx, const unsigned char *text, size_t textLen);
  int (*bjson_end_map)(void *ctx);

  int (*bjson_start_array)(void *ctx);
  int (*bjson_end_array)(void *ctx);

  int (*bjson_binary)(void *ctx, const void *buf, size_t bufLen);
}
bjson_decoderCallbacks_t;

typedef struct bjson_decodeCtx bjson_decodeCtx_t;

/*
 * Functions to create/destroy decoder context.
 * All other functions need this context to work.
 *
 * TIP#1: Each call to bjson_decoderCreate() *MUST* be followed by
 *        bjson_encoderDestroy() call.
 *
 * TIP#2: Typical usage is:
 *
 *        ctx = bjson_decoderCreate(callbacks)
 *
 *          my_function_to_read_next_chunk(buf)
 *          bjson_decoderParse(ctx, buf, bufSize)
 *
 *          my_function_to_read_next_chunk(buf)
 *          bjson_decoderParse(ctx, buf, bufSize)
 *          ...
 *
 *          bjson_decoderComplete(ctx)
 *
 *        bjson_decoderDestroy(ctx)
 *
 * TIP#3: Use callbacks parameter to catch decoded tokens
 *        (integer, string, etc.).
 */

BJSON_API bjson_decodeCtx_t *
  bjson_decoderCreate(bjson_decoderCallbacks_t *decoderCallbacks,
                      bjson_memoryFunctions_t *memoryFunctions,
                      void *callerCtx);

BJSON_API void
  bjson_decoderDestroy(bjson_decodeCtx_t *ctx);

/*
 * Functions to parse bjson stream chunk-by-chunk.
 *
 * TIP#1: Use bjson_decoderParse() to pass next bjson chunk to decoder.
 *
 * TIP#2: Use bjson_decoderComplete() to tell decoder, that all
 *        expected data are passed (end of stream).
 */

BJSON_API bjson_status_t
  bjson_decoderParse(bjson_decodeCtx_t *ctx, void *buf, size_t bufSize);

BJSON_API bjson_status_t
  bjson_decoderComplete(bjson_decodeCtx_t *ctx);

/*
 * Error handling.
 *
 * TIP: Eeach call to bjson_encoderFormatErrorMessage() *MUST* be
 *      followed by bjson_encoderFreeErrorMessage() call.
 */

BJSON_API char *
  bjson_decoderFormatErrorMessage(bjson_decodeCtx_t *ctx, int verbose);

BJSON_API void
  bjson_decoderFreeErrorMessage(bjson_decodeCtx_t *ctx, char *errorMsg);

#ifdef __cplusplus
}
#endif

#endif /* _BJSON_DECODE_H_ */