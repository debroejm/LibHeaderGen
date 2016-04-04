#pragma once

#include <map>
#include <string>

using namespace std;

void addCustomDefine(string define, string replacement);
string replaceCustomDefine(string line);

class LineEntry {
public:
    LineEntry(string line);

    LineEntry &operator=(const LineEntry rhs);

    string getData();

protected:
    string trimLine(string line);

    string line;

};