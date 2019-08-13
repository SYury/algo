template<typename T>
struct Promotion{};

template<> struct Promotion<char>{int16_t example;};
template<> struct Promotion<int8_t>{int16_t example;};
template<> struct Promotion<int16_t>{int32_t example;};
template<> struct Promotion<int32_t>{int64_t example;};
#if defined(_WIN64) || defined(__LP64__)
template<> struct Promotion<int64_t>{__int128 example;};
#endif
template<> struct Promotion<uint8_t>{uint16_t example;};
template<> struct Promotion<uint16_t>{uint32_t example;};
template<> struct Promotion<uint32_t>{uint64_t example;};
#if defined(_WIN64) || defined(__LP64__)
template<> struct Promotion<uint64_t>{unsigned __int128 example;};
#endif
template<> struct Promotion<float>{double example;};
template<> struct Promotion<double>{long double example;};

#define PROM(x) (decltype(Promotion<decltype(x)>::example)(x))
