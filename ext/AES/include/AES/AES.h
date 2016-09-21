#pragma once

#include <cstdint>
#include <cstring>

#ifdef ARDUINO
  #include <Arduino.h>
  #define STRING String
#else
  #include <string>
  #define STRING std::string

  #define PROGMEM
  #define pgm_read_byte(p) (*(p))
#endif

class SimpleVec {
public:
  SimpleVec(uint32_t length) :
    _length(0),
    _data(0)
  {
      setLength(length);
  }

  SimpleVec() :
    _length(0),
    _data(0) {}

  ~SimpleVec() {
    delete _data;
  }
  
  void setLength(uint32_t newLength) {
    if (_length != newLength) {
      _length = newLength;
      _data = new uint8_t[_length];
    }
  }
  
  void truncate(uint32_t newLength) {
    if (newLength < _length)
      _length = newLength;
  }
  
  uint32_t length() const {
    return _length;
  }
  
  const uint8_t* constData() const {
    return _data;
  }

  uint8_t* data() {
    return _data;
  }
  
  SimpleVec(uint8_t* data, uint32_t dataLength) :
    SimpleVec(dataLength)
  {
    memcpy(_data, data, dataLength);
  }

  SimpleVec(const SimpleVec& other) :
    SimpleVec(other.length())
  {
    memcpy(_data, other._data, other._length);
  }

  SimpleVec(const SimpleVec& other, uint32_t newLength, uint8_t fill=0) :
    SimpleVec(newLength)
  {
    if (newLength > other.length()) {
      memset(_data, fill, newLength);
      memcpy(_data, other._data, other._length);
    } else {
      memcpy(_data, other._data, newLength);
    }
  }
  
private:
  uint32_t _length;
  uint8_t* _data;
  

};

class AESCrypt {
public:
  AESCrypt(uint8_t* iv,
           uint8_t* key);
  AESCrypt(const STRING& iv,
           const STRING& key);
  
  void encryptString(const STRING& plainText, SimpleVec& cipher);
  STRING decryptString(const SimpleVec& cipher);
  
  void encrypt(SimpleVec& plain, SimpleVec& cipher);
  bool decrypt(const SimpleVec& cipher, SimpleVec& plain);
  
private:
  // state - array holding the intermediate results during decryption.
  typedef uint8_t state_t[4][4];
  state_t* state;
  
  // The array that stores the round keys.
  uint8_t RoundKey[176];
    
  // Initial Vector
  uint8_t Iv[16];

  void encrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length);
  void decrypt_buffer(uint8_t* output, const uint8_t* input, uint32_t length);

  void KeyExpansion(uint8_t* Key);
  void AddRoundKey(uint8_t round);
  void SubBytes(void);
  void ShiftRows(void);
  void MixColumns(void);
  void InvMixColumns(void);
  void InvSubBytes(void);
  void InvShiftRows(void);
  void Cipher(void);
  void InvCipher(void);
  void XorWithIv(uint8_t* buf);

};

/*
 The MIT License
 
 Copyright (c) 2016 Jens Krueger
 (based on "Tiny AES128 in C")
 
 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */

