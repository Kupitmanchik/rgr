#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <vector>
#include <cstdint>

std::wstring readTextFile(const std::wstring& filename);
bool writeTextFile(const std::wstring& filename, const std::wstring& content);
std::vector<unsigned char> readBinaryFile(const std::wstring& filename);
bool writeBinaryFile(const std::wstring& filename, const std::vector<unsigned char>& data);

#endif
