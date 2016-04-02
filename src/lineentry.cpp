#include "lineentry.h"

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
    return line.substr(firstIndex, line.npos);
}