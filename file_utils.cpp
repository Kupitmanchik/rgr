#include "file_utils.h"
#include <fstream>
#include <codecvt>
#include <locale>
#include <cstdint>

using namespace std;

wstring readTextFile(const wstring& filename) {
    string fname(filename.begin(), filename.end());
    wifstream wif(fname);
    wif.imbue(locale(wif.getloc(), new codecvt_utf8<wchar_t>));
    return wstring((istreambuf_iterator<wchar_t>(wif)), istreambuf_iterator<wchar_t>());
}

bool writeTextFile(const wstring& filename, const wstring& content) {
    string fname(filename.begin(), filename.end());
    wofstream wof(fname);
    wof.imbue(locale(wof.getloc(), new codecvt_utf8<wchar_t>));
    if (wof.is_open()) { wof << content; return true; }
    return false;
}

vector<unsigned char> readBinaryFile(const wstring& filename) {
    string fname(filename.begin(), filename.end());
    ifstream ifs(fname, ios::binary | ios::ate);
    if (!ifs) return {};
    uint64_t size = static_cast<uint64_t>(ifs.tellg());
    vector<unsigned char> result(size);
    ifs.seekg(0);
    ifs.read(reinterpret_cast<char*>(result.data()), size);
    return result;
}

bool writeBinaryFile(const wstring& filename, const vector<unsigned char>& data) {
    string fname(filename.begin(), filename.end());
    ofstream ofs(fname, ios::binary);
    if (!ofs) return false;
    ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
    return true;
}
