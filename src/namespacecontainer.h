#pragma once

#include <stdio.h>
#include <map>
#include <string>
#include <vector>

#include "lineentry.h"

using namespace std;

class NamespaceContainer {
public:
    NamespaceContainer() : NamespaceContainer("") { }
    NamespaceContainer(string name) : name(name) { }

    void write(FILE* file, int tabCount = 0);
    void addEntry(LineEntry entry, int priority = 0);

    NamespaceContainer* getChild(string name);

    string getName() { return name; }

protected:
    void writeTabSpacing(FILE* file, int count);

    string name;
    //vector<NamespaceContainer*> children;
    map<string, NamespaceContainer*> children;
    map<int, vector<LineEntry>> entries;

};



class NamespaceIndex {
public:
    NamespaceIndex() { }

    void add(string functionName, NamespaceContainer namespaceName);
    NamespaceContainer get(string functionName);

protected:
    map<string, NamespaceContainer> functionMap;

};