/* https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c */
#ifndef _BASE64_H_
#define _BASE64_H_

#include <vector>
#include <string>
#include <cstdint>

class Base64
{
public:
    static std::string encode(const std::vector<uint8_t>& buf);
    static std::string encode(const uint8_t* buf, uint32_t bufLen);
    static std::vector<uint8_t> decode(std::string encoded_string);
};

#endif
