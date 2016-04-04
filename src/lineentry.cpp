#include "lineentry.h"

map<string, string> defines;

void addCustomDefine(string define, string replacement) {
    defines[define] = replacement;
}

bool textChar(char c) {
    return ( (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c == 95) || (c >= 97 && c <= 122) );
}

string replaceCustomDefine(string line) {
    string result = line;
    for(auto iter : defines) {
        unsigned int index = result.find(iter.first);
        if(index != line.npos) {
            if(!textChar(line[index+iter.first.size()])) {
                result.replace(index, iter.first.size(), iter.second);
            }
        }
    }
    return result;
}

LineEntry::LineEntry(string line) : line(trimLine(line)) { }

LineEntry &LineEntry::operator=(const LineEntry rhs) {
    line = rhs.line;

    return *this;
}

string LineEntry::getData() {
    return line;
}

string LineEntry::trimLine(string line) {
    unsigned int firstIndex = 0;
    for(unsigned int i = 0; i < line.size(); i++) {
        if(line.at(i) != ' ') {
            firstIndex = i;
            break;
        }
    }
    string result = replaceCustomDefine(line.substr(firstIndex, line.npos));

    return result;
}