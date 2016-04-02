#pragma once

#include <string>

using namespace std;

class LineEntry {
public:
    LineEntry(string line);

    LineEntry &operator=(const LineEntry rhs);

    string getData();

protected:
    string trimLine(string line);

    string line;

};