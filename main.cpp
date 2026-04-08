#include <iostream>
#include <string>
#include <limits>
#include <cstdint>
#include <io.h>    
#include <fcntl.h> 
#include "table_cipher.h"
#include "tarabar.h"

using namespace std;

struct CipherContext {
    wstring input;
    wstring output;
    wstring key;
};

void displayMenu() {
    wcout << L"Выберите шифр: " << endl;
    wcout << L"Нажмите 1 для выбора шифра Простая табличная перестановка." << endl;
    wcout << L"Нажмите 2 для выбора шифра Тарабарская грамота (только согласные)." << endl;
    wcout << L"Нажмите 3 для выбора шифра Тарабарская грамота (весь алфавит)." << endl;
    wcout << L"Нажмите 0 для выхода из программы." << endl;
    wcout << L"Введите номер выбранного шифра: ";
}

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    wstring correctPassword = L"АБ421"; 
    wstring passwordOption;
    while (true) {
        wcout << L"Введите пароль: ";
        getline(wcin >> ws, passwordOption);
        
        if (passwordOption.empty()){
            wcout << L"Сообщение не может быть пустым!" << endl;
            continue;
        }
        
        if (passwordOption == correctPassword) {
            break;
        }
        wcout << L"Неверный пароль! Попробуйте снова." << endl;
    }
    
    while (true) {
        wcout << endl;
        displayMenu();
        
        wstring choiceStr;
        wcin >> choiceStr;

        if (choiceStr == L"0") {
            wcout << L"Выход из программы." << endl;
            break;
        } else if (choiceStr == L"1") {
            runTableCipher();
        } else if (choiceStr == L"2") {
            runTarabar(20); 
        } else if (choiceStr == L"3") {
            runTarabar(22); 
        } else {
            wcout << L"Ошибка: неверный ввод. Попробуйте еще раз." << endl;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        }
    }
    
    return 0;
}
