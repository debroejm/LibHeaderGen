#pragma once

#include <fstream>
#include <string>

#include "namespacecontainer.h"

using namespace std;

class OutputFile {
public:
    OutputFile(string filename);
    ~OutputFile();

    FILE* getFile() { return file; }
    NamespaceContainer* getNamespace(string name);

    void write();

protected:
    FILE* file;
    string filename;
    NamespaceContainer namespaceWrapper;

};

OutputFile* getFile(string filename);
void writeAllFiles();
void setOutputDirectory(string direct);