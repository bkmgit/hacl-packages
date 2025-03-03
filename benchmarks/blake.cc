/*
 *    Copyright 2022 Cryspen Sarl
 *
 *    Licensed under the Apache License, Version 2.0 or MIT.
 *    - http://www.apache.org/licenses/LICENSE-2.0
 *    - http://opensource.org/licenses/MIT
 */
#include "util.h"

#include "EverCrypt_Hash.h"
#include "Hacl_Hash_Blake2.h"

#ifdef HACL_CAN_COMPILE_VEC128
#include "Hacl_Hash_Blake2s_128.h"
#endif

#ifdef HACL_CAN_COMPILE_VEC256
#include "Hacl_Hash_Blake2b_256.h"
#endif

static bytes input(1000, 0x37);
static bytes key(64, 0x72);
static bytes digest2b(64, 0);
static bytes digest2s(32, 0);

// Blake2b 32-bit keyed
static void
Blake2b_32_keyed(benchmark::State& state)
{
  while (state.KeepRunning()) {
    Hacl_Blake2b_32_blake2b(digest2b.size(),
                            digest2b.data(),
                            input.size(),
                            input.data(),
                            key.size(),
                            key.data());
  }
}

// Blake2b 32-bit
static void
Blake2b_32(benchmark::State& state)
{
  while (state.KeepRunning()) {
    Hacl_Blake2b_32_blake2b(
      digest2b.size(), digest2b.data(), input.size(), input.data(), 0, NULL);
  }
}

BENCHMARK(Blake2b_32);
BENCHMARK(Blake2b_32_keyed);

// Blake2b vec256
#ifdef HACL_CAN_COMPILE_VEC256
static void
Blake2b_vec256_keyed(benchmark::State& state)
{
  cpu_init();
  if (!vec256_support()) {
    state.SkipWithError("No vec256 support");
    return;
  }

  for (auto _ : state) {
    Hacl_Blake2b_256_blake2b(digest2b.size(),
                             digest2b.data(),
                             input.size(),
                             input.data(),
                             key.size(),
                             key.data());
  }
}

static void
Blake2b_vec256(benchmark::State& state)
{
  cpu_init();
  if (!vec256_support()) {
    state.SkipWithError("No vec256 support");
    return;
  }

  for (auto _ : state) {
    Hacl_Blake2b_256_blake2b(
      digest2b.size(), digest2b.data(), input.size(), input.data(), 0, NULL);
  }
}

BENCHMARK(Blake2b_vec256);
BENCHMARK(Blake2b_vec256_keyed);
#endif // HACL_CAN_COMPILE_VEC256

// Evercrypt Blake2b
static void
Blake2b_Evercrypt(benchmark::State& state)
{
  while (state.KeepRunning()) {
    EverCrypt_Hash_hash(Spec_Hash_Definitions_Blake2B,
                        digest2b.data(),
                        input.data(),
                        input.size());
  }
}

BENCHMARK(Blake2b_Evercrypt);

#ifndef NO_OPENSSL
// OpenSSL Blake2b
static void
Openssl_Blake2b(benchmark::State& state)
{
  while (state.KeepRunning()) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (EVP_DigestInit_ex2(mdctx, EVP_blake2b512(), NULL) != 1) {
      state.SkipWithError("Error in EVP_DigestInit_ex2");
      EVP_MD_CTX_free(mdctx);
      break;
    }
    if (EVP_DigestUpdate(mdctx, input.data(), input.size()) != 1) {
      state.SkipWithError("Error in EVP_DigestUpdate");
      EVP_MD_CTX_free(mdctx);
      break;
    }
    unsigned int md_len = 0;
    if (EVP_DigestFinal_ex(mdctx, digest2b.data(), &md_len) != 1 ||
        md_len != 64) {
      state.SkipWithError("Error in EVP_DigestFinal_ex");
      EVP_MD_CTX_free(mdctx);
      break;
    }
    EVP_MD_CTX_free(mdctx);
  }
}

BENCHMARK(Openssl_Blake2b);
#endif

// Blake2s 32-bit keyed
static void
Blake2s_32_keyed(benchmark::State& state)
{
  for (auto _ : state) {
    Hacl_Blake2s_32_blake2s(digest2s.size(),
                            digest2s.data(),
                            input.size(),
                            input.data(),
                            key.size(),
                            key.data());
  }
}

// Blake2s 32-bit
static void
Blake2s_32(benchmark::State& state)
{
  for (auto _ : state) {
    Hacl_Blake2s_32_blake2s(
      digest2s.size(), digest2s.data(), input.size(), input.data(), 0, NULL);
  }
}

BENCHMARK(Blake2s_32);
BENCHMARK(Blake2s_32_keyed);

// Blake2s vec128
#ifdef HACL_CAN_COMPILE_VEC128
static void
Blake2s_vec128_keyed(benchmark::State& state)
{
  cpu_init();
  if (!vec128_support()) {
    state.SkipWithError("No vec128 support");
    return;
  }

  for (auto _ : state) {
    Hacl_Blake2s_128_blake2s(digest2s.size(),
                             digest2s.data(),
                             input.size(),
                             input.data(),
                             key.size(),
                             key.data());
  }
}

static void
Blake2s_vec128(benchmark::State& state)
{
  cpu_init();
  if (!vec128_support()) {
    state.SkipWithError("No vec128 support");
    return;
  }

  for (auto _ : state) {
    Hacl_Blake2s_128_blake2s(
      digest2s.size(), digest2s.data(), input.size(), input.data(), 0, NULL);
  }
}

BENCHMARK(Blake2s_vec128);
BENCHMARK(Blake2s_vec128_keyed);
#endif // HACL_CAN_COMPILE_VEC128

// Evercrypt Blake2s
static void
Blake2s_Evercrypt(benchmark::State& state)
{
  EverCrypt_AutoConfig2_init();
  while (state.KeepRunning()) {
    EverCrypt_Hash_hash(Spec_Hash_Definitions_Blake2S,
                        digest2s.data(),
                        input.data(),
                        input.size());
  }
}
BENCHMARK(Blake2s_Evercrypt);

#ifndef NO_OPENSSL
// OpenSSL Blake2s
static void
Openssl_Blake2s(benchmark::State& state)
{
  while (state.KeepRunning()) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (EVP_DigestInit_ex2(mdctx, EVP_blake2s256(), NULL) != 1) {
      state.SkipWithError("Error in EVP_DigestInit_ex2");
      EVP_MD_CTX_free(mdctx);
      break;
    }
    if (EVP_DigestUpdate(mdctx, input.data(), input.size()) != 1) {
      state.SkipWithError("Error in EVP_DigestUpdate");
      EVP_MD_CTX_free(mdctx);
      break;
    }
    unsigned int md_len = 0;
    if (EVP_DigestFinal_ex(mdctx, digest2s.data(), &md_len) != 1 ||
        md_len != 32) {
      state.SkipWithError("Error in EVP_DigestFinal_ex");
      EVP_MD_CTX_free(mdctx);
      break;
    }
    EVP_MD_CTX_free(mdctx);
  }
}

BENCHMARK(Openssl_Blake2s);
#endif

BENCHMARK_MAIN();
