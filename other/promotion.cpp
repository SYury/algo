template<typename T>
struct Promotion{};

template<> struct Promotion<char>{typedef int16_t promoted;};
template<> struct Promotion<int8_t>{typedef int16_t promoted;};
template<> struct Promotion<int16_t>{typedef int32_t promoted;};
template<> struct Promotion<int32_t>{typedef int64_t promoted;};
#if defined(_WIN64) || defined(__LP64__)
template<> struct Promotion<int64_t>{typedef __int128 promoted;};
#endif
template<> struct Promotion<uint8_t>{typedef uint16_t promoted;};
template<> struct Promotion<uint16_t>{typedef uint32_t promoted;};
template<> struct Promotion<uint32_t>{typedef uint64_t promoted;};
#if defined(_WIN64) || defined(__LP64__)
template<> struct Promotion<uint64_t>{typedef unsigned __int128 promoted;};
#endif
template<> struct Promotion<float>{typedef double promoted;};
template<> struct Promotion<double>{typedef long double promoted;};

#define PROM(x) ((typename Promotion<typename std::__decay_and_strip<decltype(x)>::__type>::promoted)(x))
