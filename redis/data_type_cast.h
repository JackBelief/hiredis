#pragma once

#include <string>

template<typename T>
struct StringCast
{

};

#define CAST_STRING_TO_SIMPLE_DATA_TYPE(T, STR_TO_XXX)              \
template <>                                                         \
struct StringCast<T>                                                \
{                                                                   \
    T operator() (const std::string& str) const                     \
    try                                                             \
    {                                                               \
        return STR_TO_XXX(str.c_str());                             \
    }                                                               \
    catch (...)                                                     \
    {                                                               \
        std::cout << "cast error" << std::endl;                     \
    }                                                               \
};

inline unsigned long long cast_str_to_ull(const char *str)
{
    return std::strtoull(str, NULL, 0);
}

inline long long cast_str_to_ll(const char *str)
{
    return std::strtoll(str, NULL, 0);
}

inline double cast_str_to_d(const char* str)
{
    return std::strtod(str, NULL);
}

CAST_STRING_TO_SIMPLE_DATA_TYPE(bool,               cast_str_to_ull);

CAST_STRING_TO_SIMPLE_DATA_TYPE(char,               cast_str_to_ll);
CAST_STRING_TO_SIMPLE_DATA_TYPE(signed char,        cast_str_to_ll);
CAST_STRING_TO_SIMPLE_DATA_TYPE(unsigned char,      cast_str_to_ull);

CAST_STRING_TO_SIMPLE_DATA_TYPE(signed short,       cast_str_to_ll);
CAST_STRING_TO_SIMPLE_DATA_TYPE(unsigned short,     cast_str_to_ull);

CAST_STRING_TO_SIMPLE_DATA_TYPE(signed int,         cast_str_to_ll);
CAST_STRING_TO_SIMPLE_DATA_TYPE(unsigned int,       cast_str_to_ull);

CAST_STRING_TO_SIMPLE_DATA_TYPE(signed long,        cast_str_to_ll);
CAST_STRING_TO_SIMPLE_DATA_TYPE(unsigned long,      cast_str_to_ull);

CAST_STRING_TO_SIMPLE_DATA_TYPE(signed long long,   cast_str_to_ll);
CAST_STRING_TO_SIMPLE_DATA_TYPE(unsigned long long, cast_str_to_ull);

CAST_STRING_TO_SIMPLE_DATA_TYPE(float,              cast_str_to_d);
CAST_STRING_TO_SIMPLE_DATA_TYPE(double,             cast_str_to_d);


template <typename T>
struct StringCastWrapper
{
    T operator()(const std::string& str)
    {
        return StringCast<T>()(str);
    }
};

template<>
struct StringCastWrapper<std::string>
{
    std::string operator() (const std::string& str)
    {
        return std::move(str);
    }
};


