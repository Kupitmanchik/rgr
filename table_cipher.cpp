#include "table_cipher.h"
#include "file_utils.h"
#include <iostream>
#include <limits>
#include <ctime>
#include <vector>
#include <cstdint>

using namespace std;

wstring encryptTable(const wstring& text, uint64_t rows)
{
    if (rows == 0)
    {
        return text;
    }
    uint64_t len = text.length();
    uint64_t cols = (len + rows - 1) / rows;
    wstring output = L"";
    for (uint64_t i = 0; i < rows; ++i)
    {
        for (uint64_t j = 0; j < cols; ++j)
        {
            uint64_t index = j * rows + i;
            if (index < len)
            {
                output += text[index];
            }
            else
            {
                output += L' ';
            }
        }
    }
    return output;
}
wstring decryptTable(const wstring& text, uint64_t rows)
{
    if (rows == 0)
    {
        return text;
    }
    uint64_t len = text.length();
    uint64_t cols = (len + rows - 1) / rows;
    vector<wstring> table(rows, wstring(cols, L' '));
    uint64_t k = 0;
    for (uint64_t i = 0; i < rows; ++i)
    {
        for (uint64_t j = 0; j < cols; ++j)
        {
            if (k < len)
            {
                table[i][j] = text[k++];
            }
        }
    }
    wstring output = L"";
    for (uint64_t j = 0; j < cols; ++j)
    {
        for (uint64_t i = 0; i < rows; ++i)
        {
            output += table[i][j];
        }
    }
    return output;
}
vector<unsigned char> encryptTableBinary(const vector<unsigned char>& data, uint64_t rows)
{
    if (rows == 0 || data.empty())
    {
        return data;
    }
    uint64_t len = data.size();
    uint64_t cols = (len + rows - 1) / rows;
    vector<unsigned char> output;
    output.reserve(rows * cols);
    for (uint64_t i = 0; i < rows; ++i)
    {
        for (uint64_t j = 0; j < cols; ++j)
        {
            uint64_t index = j * rows + i;
            if (index < len)
            {
                output.push_back(data[index]);
            }
            else
            {
                output.push_back(0);
            }
        }
    }
    return output;
}
vector<unsigned char> decryptTableBinary(const vector<unsigned char>& data, uint64_t rows)
{
    if (rows == 0 || data.empty())
    {
        return data;
    }
    uint64_t len = data.size();
    uint64_t cols = (len + rows - 1) / rows;
    vector<vector<unsigned char>> table(rows, vector<unsigned char>(cols, 0));
    uint64_t k = 0;
    for (uint64_t i = 0; i < rows; ++i)
    {
        for (uint64_t j = 0; j < cols; ++j)
        {
            if (k < len)
            {
                table[i][j] = data[k++];
            }
        }
    }
    vector<unsigned char> output;
    output.reserve(len);
    for (uint64_t j = 0; j < cols; ++j)
    {
        for (uint64_t i = 0; i < rows; ++i)
        {
            output.push_back(table[i][j]);
        }
    }
    return output;
}
void runTableCipher()
{
    srand(static_cast<unsigned int>(time(0)));
    uint64_t choice = 0;
    while (true)
    {
        wcout << endl;
        wcout << L"--- Простая табличная перестановка ---" << endl;
        wcout << L"Выберите объект для шифрования: " << endl;
        wcout << L"Нажмите 1 для ввода текста с консоли. " << endl;
        wcout << L"Нажмите 2 для чтения текста из файла (.txt)." << endl;
        wcout << L"Нажмите 3 для чтения изображения (.png)." << endl;
        wcout << L"Нажмите 4 для генерации случайного ключа." << endl;
        wcout << L"Нажмите 0 для возврата в главное меню." << endl;
        wcout << L"Введите номер выбранного объекта: ";
        if (!(wcin >> choice))
        {
            wcout << L"Ошибка: введите число!" << endl;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            continue;
        }
        if (choice == 0)
        {
            break;
        }
        if (choice == 4)
        {
            uint64_t genKey = rand() % 10 + 2;
            wcout << L"Сгенерированный ключ (количество строк): " << genKey << endl;
            continue;
        }
        if (choice < 1 || choice > 3)
        {
            wcout << L"Ошибка: неверный пункт меню." << endl;
            continue;
        }
        uint64_t key;
        while (true)
        {
            wcout << L"Введите ключ (положительное число): ";
            if (wcin >> key && key > 0)
            {
                break;
            }
            wcout << L"Ошибка: ключ должен быть положительным целым числом!" << endl;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        }
        if (choice == 1)
        {
            wstring text;
            wcout << L"Введите сообщение: ";
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            getline(wcin, text);

            wstring encrypted = encryptTable(text, key);
            wstring decrypted = decryptTable(encrypted, key);

            wcout << L"Зашифрованный текст: " << encrypted << endl;
            wcout << L"Расшифрованный текст: " << decrypted << endl;
        }
        else if (choice == 2)
        {
            wstring inFile, encFile, decFile;
            wcout << L"Введите имя входного файла: ";
            wcin >> inFile;
            wcout << L"Введите имя выходного файла для шифрования: ";
            wcin >> encFile;
            wcout << L"Введите имя выходного файла для дешифрования: ";
            wcin >> decFile;
            wstring text = readTextFile(inFile);
            if (text.empty())
            {
                wcout << L"Ошибка: файл пуст или не найден." << endl;
                continue;
            }
            wstring encText = encryptTable(text, key);
            writeTextFile(encFile, encText);
            wcout << L"Текст успешно зашифрован и записан в: " << encFile << endl;

            wstring decText = decryptTable(encText, key);
            writeTextFile(decFile, decText);
            wcout << L"Текст успешно расшифрован и записан в: " << decFile << endl;
        }
        else if (choice == 3)
        {
            wstring inFile, encFile, decFile;
            wcout << L"Введите имя входного изображения: ";
            wcin >> inFile;
            wcout << L"Введите имя выходного файла для шифрования: ";
            wcin >> encFile;
            wcout << L"Введите имя выходного файла для дешифрования: ";
            wcin >> decFile;
            auto data = readBinaryFile(inFile);
            if (data.empty())
            {
                wcout << L"Ошибка: файл изображения не найден." << endl;
                continue;
            }
            auto encData = encryptTableBinary(data, key);
            writeBinaryFile(encFile, encData);
            wcout << L"Изображение зашифровано и записано в: " << encFile << endl;
            auto decData = decryptTableBinary(encData, key);
            writeBinaryFile(decFile, decData);
            wcout << L"Изображение расшифровано и записано в: " << decFile << endl;
        }
    }
}
