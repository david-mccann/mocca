#include "mocca/base/Encryption.h"

#include "AES/AES.h"
#include "base64/base64.h"

#include <algorithm>
#include <array>
#include <random>
#include <cstring>

std::string mocca::impl::createIv() {
    std::array<uint16_t, 8> randomData;
    std::uniform_int_distribution<uint16_t> dist;
    std::default_random_engine engine(std::random_device{}());
    auto gen = [&]() { return dist(engine); };
    std::generate(begin(randomData), end(randomData), gen);

    std::string iv(16, '\0');
    for (int i = 0; i < 8; ++i) {
        iv[2 * i] = static_cast<uint8_t>(randomData[i] >> 8);
        iv[2 * i + 1] = static_cast<uint8_t>(randomData[i] & 0xFF);
    }
    return iv;
}

std::string mocca::encryptString(const std::string& plainText, const std::string& key) {
    std::string iv = impl::createIv();
    AESCrypt enc(iv, key);
    SimpleVec cipher;
    enc.encryptString(plainText, cipher);
    SimpleVec result(cipher.length() + 16);
    // prepend iv to encrypted data
    std::memcpy(result.data(), iv.data(), iv.size());
    std::memcpy(result.data() + 16, cipher.constData(), cipher.length());
    return base64_encode(result.constData(), result.length());
}

std::string mocca::decryptString(const std::string& encodedText, const std::string& key) {
    std::string data = base64_decode(encodedText);
    // separate iv and encrypted data
    std::string iv = data.substr(0, 16);
    SimpleVec cipher(reinterpret_cast<uint8_t*>(const_cast<char*>(data.data() + 16)), data.size() - 16);
    AESCrypt dec(iv, key);
    return dec.decryptString(cipher);
}