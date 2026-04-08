#ifndef TARABAR_H
#define TARABAR_H

#include <string>
#include <vector>
#include <cstdint>

// Используем uint64_t для версии и ключа
void runTarabar(uint64_t version);
std::wstring processTarabarText(const std::wstring& text, uint64_t version, uint64_t key, bool encrypt);
std::vector<unsigned char> processTarabarBinary(const std::vector<unsigned char>& data, uint64_t key, bool encrypt);

#endif
