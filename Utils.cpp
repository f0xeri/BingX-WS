//
// Created by user on 09.02.2024.
//

#include "Utils.hpp"

std::string decompressGZip(const std::string& data) {
    namespace bio = boost::iostreams;
    std::istringstream compressed(data);
    bio::filtering_istreambuf in;
    in.push(bio::gzip_decompressor());
    in.push(compressed);
    std::ostringstream origin;
    bio::copy(in, origin);
    return origin.str();
}

std::string calcHmacSHA256(std::string_view decodedKey, std::string_view msg) {
    std::array<uint8_t, EVP_MAX_MD_SIZE> hash{};
    uint32_t hashLen;
    HMAC(
            EVP_sha256(),
            decodedKey.data(), static_cast<int>(decodedKey.size()),
            reinterpret_cast<uint8_t const*>(msg.data()), static_cast<int>(msg.size()),
            hash.data(), &hashLen
    );
    std::stringstream out;
    for (uint32_t i = 0; i < hashLen; i++) {
        out << std::setfill('0') << std::setw(2) << std::right << std::hex << static_cast<int>(hash[i]);
    }
    return out.str();
}

void fail(boost::beast::error_code ec, char const* what) {
    std::cerr << what << ": " << ec.message() << "\n";
}