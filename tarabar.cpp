#include "tarabar.h"
#include "file_utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <cwctype>
#include <limits>
#include <ctime>
#include <cstdint>

using namespace std;

// === 1. КОНСТАНТЫ (должны быть в самом верху) ===

// 5.20 Классика
const wstring RU_20_1 = L"БВГДЖЗКЛМНбвгджзклмн";
const wstring RU_20_2 = L"ЩШЧЦХФТСРПщшчцхфтсрп";
const wstring EN_20_1 = L"BCDFGHJKLMbcdfghjklm";
const wstring EN_20_2 = L"NPQRSTVWXZnpqrstvwxz";
const wstring VOWELS_20 = L"АЕЁИОУЫЭЮЯаеёиоуыэюяAEIOUYaeiouy";

// 5.22
const wstring RU_522_1 = L"БВГДЖЗКЛМНПРСТФбвгджзклмнпрстф";
const wstring RU_522_2 = L"АЕЁИОУЫЭЮЯЩШЧЦХаеёиоуыэюящшчцх";
const wstring EN_522_1 = L"BCDFGHJKLMNPQbcdfghjklmnpq";
const wstring EN_522_2 = L"AEIOURSTVWXYZaeiourstvwxyz";

const wstring RU_EXCEPT = L"ЙЪЬйъь"; 
const wstring SYMBOLS   = L"0123456789!@#$%^&*()-_=+[]{};:'\",.<>/?|\\";

// === 2. ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===

wchar_t safeShift(wchar_t c, const wstring& alphabet, uint64_t n, bool encrypt) {
    if (alphabet.empty()) return 0;
    size_t pos = alphabet.find(c);
    if (pos == wstring::npos) return 0;
    
    int size = (int)alphabet.size();
    int shift = encrypt ? (int)(n % size) : -(int)(n % size);
    int newPos = (int)(pos + shift) % size;
    if (newPos < 0) newPos += size;
    
    return alphabet[newPos];
}

// === 3. ОСНОВНЫЕ ФУНКЦИИ ОБРАБОТКИ ===

wstring processTarabarText(const wstring& text, uint64_t version, uint64_t key, bool encrypt) {
    wstring result = L"";
    for (wchar_t c : text) {
        if (iswspace(c)) { 
            result += c; 
            continue; 
        }

        size_t pos;
        if (version == 2) { // 5.20 Классика (вызывается из main как 2)
            if ((pos = RU_20_1.find(c)) != wstring::npos) result += RU_20_2[pos];
            else if ((pos = RU_20_2.find(c)) != wstring::npos) result += RU_20_1[pos];
            else if ((pos = EN_20_1.find(c)) != wstring::npos) result += EN_20_2[pos];
            else if ((pos = EN_20_2.find(c)) != wstring::npos) result += EN_20_1[pos];
            else {
                wchar_t r = safeShift(c, VOWELS_20, key, encrypt);
                if (r == 0) r = safeShift(c, SYMBOLS, key, encrypt);
                result += (r != 0) ? r : c;
            }
        } 
        else { // 5.22 (вызывается из main как 3)
            if ((pos = RU_522_1.find(c)) != wstring::npos) result += RU_522_2[pos];
            else if ((pos = RU_522_2.find(c)) != wstring::npos) result += RU_522_1[pos];
            else if ((pos = EN_522_1.find(c)) != wstring::npos) result += EN_522_2[pos];
            else if ((pos = EN_522_2.find(c)) != wstring::npos) result += EN_522_1[pos];
            else {
                wchar_t r = safeShift(c, RU_EXCEPT, key, encrypt);
                if (r == 0) r = safeShift(c, SYMBOLS, key, encrypt);
                result += (r != 0) ? r : c;
            }
        }
    }
    return result;
}

vector<unsigned char> processTarabarBinary(const vector<unsigned char>& data, uint64_t key, bool encrypt) {
    vector<unsigned char> result = data;
    unsigned char byteKey = static_cast<unsigned char>(key % 256);
    for (auto& b : result) {
        if (encrypt) b += byteKey;
        else b -= byteKey;
    }
    return result;
}

// === 4. ИНТЕРФЕЙС ===

void runTarabar(uint64_t version) {
    srand(static_cast<unsigned int>(time(0))); 
    uint64_t choice = 0;
    
    while (true) {
        wcout << endl;
        wcout << L"--- Тарабарская грамота (" << (version == 2 ? L"согласные" : L"весь алфавит") << L") ---" << endl;
        wcout << L"1. Консоль\n2. Файл (Текст)\n3. Файл (Изобр.)\n4. Генерация ключа\n0. Назад\nВыбор: ";
        
        if (!(wcin >> choice)) {
            wcout << L"Ошибка: введите число!" << endl;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            continue;
        }

        if (choice == 0) break;

        if (choice == 4) {
            uint64_t type;
            wcout << L"Тип ключа (1-текст, 2-бинар): ";
            wcin >> type;
            uint64_t genKey = rand() % 10 + 1;
            wcout << L"Сгенерированный ключ: " << genKey << endl;
            continue;
        }

        uint64_t key;
        wcout << L"Ключ n: ";
        if (!(wcin >> key)) {
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            continue;
        }
        wcin.ignore(numeric_limits<streamsize>::max(), L'\n');

        if (choice == 1) {
            wstring text;
            wcout << L"Введите текст: ";
            getline(wcin >> ws, text);
            wstring enc = processTarabarText(text, version, key, true);
            wcout << L"Результат: " << enc << endl;
            wcout << L"Обратно:   " << processTarabarText(enc, version, key, false) << endl;
        } 
        else if (choice == 2) {
            uint64_t mode; wstring inF, outF;
            wcout << L"1. Зашифровать, 2. Расшифровать: "; wcin >> mode;
            wcin.ignore(1000, L'\n');
            wcout << L"Входной файл: "; getline(wcin, inF);
            wcout << L"Выходной файл: "; getline(wcin, outF);

            wstring data = readTextFile(inF);
            if (!data.empty()) {
                writeTextFile(outF, processTarabarText(data, version, key, mode == 1));
                wcout << L"Готово." << endl;
            }
        }
        else if (choice == 3) {
            uint64_t mode; wstring inF, outF;
            wcout << L"1. Зашифровать, 2. Расшифровать: "; wcin >> mode;
            wcin.ignore(1000, L'\n');
            wcout << L"Файл изображения: "; getline(wcin, inF);
            wcout << L"Выходной файл: "; getline(wcin, outF);

            auto data = readBinaryFile(inF);
            if (!data.empty()) {
                auto resultData = processTarabarBinary(data, key, mode == 1);
                writeBinaryFile(outF, resultData);
                wcout << L"Готово." << endl;
            }
        }
    }
}
