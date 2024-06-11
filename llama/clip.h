/**
 * llama.cpp - git ee459f40f65810a810151b24eba5b8bd174ceffe
 *
 * MIT License
 *
 * Copyright (c) 2023-2024 The ggml authors
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

#ifndef CLIP_H
#define CLIP_H

#include <stddef.h>
#include <stdint.h>

#ifdef LLAMA_SHARED
#    if defined(_WIN32) && !defined(__MINGW32__)
#        ifdef LLAMA_BUILD
#            define CLIP_API __declspec(dllexport)
#        else
#            define CLIP_API __declspec(dllimport)
#        endif
#    else
#        define CLIP_API __attribute__ ((visibility ("default")))
#    endif
#else
#    define CLIP_API
#endif

struct clip_ctx;

#ifdef __cplusplus
extern "C" {
#endif

struct clip_ctx;

struct clip_image_u8_batch {
    struct clip_image_u8 * data;
    size_t size;
};

struct clip_image_f32_batch {
    struct clip_image_f32 * data;
    size_t size;
};

CLIP_API struct clip_ctx * clip_model_load    (const char * fname, int verbosity);
CLIP_API struct clip_ctx * clip_model_load_cpu(const char * fname, int verbosity);

CLIP_API void clip_free(struct clip_ctx * ctx);

CLIP_API size_t clip_embd_nbytes(const struct clip_ctx * ctx);

CLIP_API int32_t clip_image_size (const struct clip_ctx * ctx);
CLIP_API int32_t clip_patch_size (const struct clip_ctx * ctx);
CLIP_API int32_t clip_hidden_size(const struct clip_ctx * ctx);

// TODO: should be enum, not string
CLIP_API const char * clip_patch_merge_type(const struct clip_ctx * ctx);

CLIP_API const int32_t * clip_image_grid(const struct clip_ctx * ctx);

CLIP_API int clip_n_patches    (const struct clip_ctx * ctx);
CLIP_API int clip_n_mmproj_embd(const struct clip_ctx * ctx);

CLIP_API struct clip_image_u8  * clip_image_u8_init ();
CLIP_API struct clip_image_f32 * clip_image_f32_init();

CLIP_API void clip_image_u8_free (struct clip_image_u8  * img);
CLIP_API void clip_image_f32_free(struct clip_image_f32 * img);
CLIP_API void clip_image_u8_batch_free (struct clip_image_u8_batch  * batch);
CLIP_API void clip_image_f32_batch_free(struct clip_image_f32_batch * batch);

CLIP_API bool clip_image_load_from_file(const char * fname, struct clip_image_u8 * img);

/** interpret bytes as an image file with length bytes_length, and use the result to populate img */
CLIP_API bool clip_image_load_from_bytes(const unsigned char * bytes, size_t bytes_length, struct clip_image_u8 * img);

/** preprocess img and store the result in res_imgs, pad_to_square may be overridden to false depending on model configuration */
CLIP_API bool clip_image_preprocess(struct clip_ctx * ctx, const struct clip_image_u8 * img, struct clip_image_f32_batch * res_imgs );

CLIP_API struct ggml_tensor * clip_get_newline_tensor(const struct clip_ctx * ctx);

CLIP_API bool clip_image_encode      (struct clip_ctx * ctx, int n_threads, struct clip_image_f32 * img, float * vec);
CLIP_API bool clip_image_batch_encode(struct clip_ctx * ctx, int n_threads, const struct clip_image_f32_batch * imgs, float * vec);

CLIP_API bool clip_model_quantize(const char * fname_inp, const char * fname_out, int itype);

#ifdef __cplusplus
}
#endif

#endif // CLIP_H
