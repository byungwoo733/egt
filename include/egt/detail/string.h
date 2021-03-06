/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef EGT_DETAIL_STRING_H
#define EGT_DETAIL_STRING_H

/**
 * @file
 * @brief String utilities.
 */

#include <egt/detail/meta.h>
#include <egt/detail/stringhash.h>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace egt
{
inline namespace v1
{
namespace detail
{

/**
 * Replace all in string.
 */
EGT_API std::string replace_all(std::string str, const std::string& from,
                                const std::string& to);

/**
 * Strip the specified characters from the string.
 */
EGT_API void strip(std::string& str, const std::string& t = " \t\n\r");

/**
 * Truncate a string if applicable and optionally add ellipse to end.
 */
EGT_API std::string truncate(const std::string& str, size_t width,
                             bool ellipsis = true);

/**
 * Trim delimiters off the right side of a std::string
 */
EGT_API std::string rtrim(const std::string& source, const std::string& t = " \t\n\r");

/**
 * Trim delimiters off the left side of a std::string
 */
EGT_API std::string ltrim(const std::string& source, const std::string& t = " \t\n\r");

/**
 * Trim delimiters off both sides of a std::string
 */
EGT_API std::string trim(const std::string& source, const std::string& t = " \t\n\r");

/**
 * Format a float/double to a fixed precision and return as a string.
 */
template<class T>
std::string format(T value, int precision = 2)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(precision) << value;
    return stream.str();
}

/**
 * Tokenize a std::string
 */
EGT_API void tokenize(const std::string& str, char delimiter, std::vector<std::string>& tokens);

/**
 * Join each item of a container with the specified delimiter between each item.
 */
template<class T>
void join(std::ostream& os, const T& container, const std::string& delimiter = ",")
{
    bool first = true;
    for (auto& item : container)
    {
        if (first)
            first = false;
        else
            os << delimiter;
        os << item;
    }
}

/**
 * Convert a string to lowercase.
 * @note Not UTF-8 aware.
 */
EGT_API void tolower(std::string& s);

/**
 * Convert a string to lowercase.
 * @note Not UTF-8 aware.
 */
EGT_API void toupper(std::string& s);

/**
 * Convert a type to a std::string using std::ostringstream.
 */
template<class T>
std::string to_string(const T& x)
{
    std::ostringstream ss;
    ss << x;
    return ss.str();
}

/**
 * Convert a bool to a lexical std::string.
 */
template<>
inline std::string to_string(const bool& x)
{
    return x ? "true" : "false";
}

/**
 * Convert a lexical std::string to a bool.
 */
EGT_API bool from_string(const std::string& x);

/**
 * Like, but not the same as strtoul().
 *
 * The idea here is a constexpr function to convert a character string to an
 * integer without a whole lot of error checking.
 */
constexpr unsigned int hextoul(const char* str, unsigned int base = 16)
{
    unsigned int result = 0;
    auto s = str;

    while (*s == '#' || *s == ' ' || *s == '\t' || *s == '\n' || *s == '\v' || *s == '\f' || *s == '\r')
        s++;

    while (*s)
    {
        unsigned int c = *s++;
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'F')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'f')
            c -= 'a' - 10;
        else
            break;
        result *= base;
        result += c;
    }

    return result;
}

}
}
}

#endif
