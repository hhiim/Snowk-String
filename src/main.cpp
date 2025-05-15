#include "string.hpp"
#include "UTF16.h"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <string>

using namespace std;
using namespace Snowk;
using namespace Snowk::internal;

int main() {
    ifstream file;
    file.open("/home/knight/桌面/Projects/String/test/test.txt", ios::in);
    assert(file.is_open());

    file.seekg(0, ios::end);
    streampos size = file.tellg();

    // 将 streampos 转换为 size_t
    size_t fileSize = static_cast<size_t>(size);
    std::cout << "File size: " << fileSize << " bytes" << std::endl;

    char *buffer = new char[fileSize]{};
    file.seekg(0, ios::beg);
    file.read(buffer, fileSize);

    Snowk::string<UTF16, endian::big> str(buffer);
    Snowk::string<UTF16, endian::native> str2(str);
    delete [] buffer;
    file.close();
}
