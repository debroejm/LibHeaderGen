#include "outputfile.h"

map<string, OutputFile*> fileMapping;
string outputDirectory = "";

OutputFile* getFile(string filename) {
    if(fileMapping[filename] == NULL)
        fileMapping[filename] = new OutputFile(filename);
    return fileMapping[filename];
}

void writeAllFiles() {
    for(auto &iter : fileMapping) {
        iter.second->write();
    }
}

OutputFile::OutputFile(string filename)
        : filename(filename), namespaceWrapper("") {
    file = fopen((outputDirectory+filename+".h").c_str(), "w");
    fprintf(file, "#pragma once\n\n");
}

OutputFile::~OutputFile() {
    fclose(file);
}

void OutputFile::write() {
    namespaceWrapper.write(file);
}

NamespaceContainer* OutputFile::getNamespace(string name) {
    return namespaceWrapper.getChild(name);
}

void setOutputDirectory(string direct) {
    outputDirectory = direct;
}