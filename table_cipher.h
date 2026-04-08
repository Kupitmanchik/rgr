#ifndef TABLE_CIPHER_H
#define TABLE_CIPHER_H

#include <string>
#include <vector>
#include <cstdint>

void runTableCipher();
std::wstring encryptTable(const std::wstring& text, uint64_t rows);
std::wstring decryptTable(const std::wstring& text, uint64_t rows);
std::vector<unsigned char> encryptTableBinary(const std::vector<unsigned char>& data, uint64_t rows);
std::vector<unsigned char> decryptTableBinary(const std::vector<unsigned char>& data, uint64_t rows);

#endif
