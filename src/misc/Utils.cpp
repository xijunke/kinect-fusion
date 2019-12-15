#include <string>
#include <fstream>
#include <sstream>

#include "Utils.h"

string Utils::readTextFile(string path){
    ifstream file(path, ios::in);

    if (!file.is_open())
        throw runtime_error("Cannot open file '" + path + "'");

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
