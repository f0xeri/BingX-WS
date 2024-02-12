//
// Created by user on 09.02.2024.
//

#ifndef TRADING_UTILS_HPP
#define TRADING_UTILS_HPP

#include <string>
#include <sstream>
#include <array>
#include <iomanip>
#include <iostream>
#include <boost/beast/core/error.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/json.hpp>
#include <openssl/hmac.h>

std::string decompressGZip(const std::string& data);
std::string calcHmacSHA256(std::string_view decodedKey, std::string_view msg);
void fail(boost::beast::error_code ec, char const* what);
#endif //TRADING_UTILS_HPP
