#pragma once

#ifdef VORTEX_DEBUG

#ifndef VORTEX_DEBUG_BREAK
  #define VORTEX_DEBUG_BREAK __debugbreak();
#endif

#ifndef VORTEX_ASSERT
  #include <cstdio>
  #include <filesystem>

  #define VORTEX_ASSERT(x) \
    if(!(x)) {                                   \
        fprintf(stderr,"%s - Assertion failed at: %s:%d\n",#x, std::filesystem::path(__FILE__).filename().string().c_str(),__LINE__); \
        VORTEX_DEBUG_BREAK                           \
    }

  #define VORTEX_ASSERT_MSG(x, fmt, ...) \
    if(!(x)) {                                   \
        fprintf(stderr,(fmt), ##__VA_ARGS__);        \
        fprintf(stderr," - ");                       \
        fprintf(stderr,"Assertion failed at: %s:%d\n", std::filesystem::path(__FILE__).filename().string().c_str(),__LINE__); \
        VORTEX_DEBUG_BREAK                           \
    }

#endif

#else

#define VORTEX_DEBUG_BREAK
#define VORTEX_ASSERT(x)
#define VORTEX_ASSERT_MSG(x, fmt, ...)

#endif

#ifndef VORTEX_STATIC_ASSERT
  #define VORTEX_STATIC_ASSERT(x) static_assert( x , #x );
  #define VORTEX_STATIC_ASSERT_MSG(x, msg) static_assert( x , msg );
#endif
