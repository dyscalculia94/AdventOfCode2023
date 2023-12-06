#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

inline std::vector<std::string> split(std::string str, std::string del)
{
    size_t start = 0, end, delLen = del.length();
    std::string token;
    std::vector<std::string> res;

    while ((end = str.find(del, start)) != std::string::npos) {
        token = str.substr (start, end - start);
        start = end + delLen;
        res.push_back (token);
    }

    res.push_back (str.substr (start));
    return res;
}

#endif