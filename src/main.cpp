#include <dirent.h>
#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "namespacecontainer.h"
#include "outputfile.h"

using namespace std;

// Prototypes
bool scanDirectory(const char* dirName, bool recursive, int depth = 1);
bool isHeaderFile(const char* fileName, int nameLen);
void scanFile(const char* fileName);
void parsePragmaLine(string line);
void parsePragma(string command, string args[], int argc);

// Flags
bool singleGrab = false;
bool multiGrab = false;

// Debug
string activeFilename = "";
int activeLineNum = 0;

// Reading Data
NamespaceContainer* activeNamespace = NULL;
int activePriority = 0;

int main(int argc, char* argv[]) {

    char* directoryName = NULL;
    bool recursive = false;
    char* outputDirName = NULL;

    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-p") == 0) {
            if(i+1 >= argc) {
                cerr << "ERR: Missing argument for -p" << endl;
                return EXIT_FAILURE;
            } else {
                directoryName = argv[i+1];
                i++;
                continue;
            }
        } else if(strcmp(argv[i], "-r") == 0) {
            recursive = true;
            continue;
        } else if(strcmp(argv[i], "-o") == 0) {
            if(i+1 >= argc) {
                cerr << "ERR: Missing argument for -o" << endl;
                return EXIT_FAILURE;
            } else {
                outputDirName = argv[i+1];
                i++;
                continue;
            }
        }
    }

    if(outputDirName != NULL) {
        DIR* dir = opendir(outputDirName);
        if(dir == NULL) {
            cerr << "ERR: No such output directory '" << outputDirName << "'" << endl;
            return EXIT_FAILURE;
        } else {
            setOutputDirectory(string(outputDirName) + "\\");
        }
    }

    if(directoryName != NULL) {
        if(!scanDirectory(directoryName, recursive)) {
            cerr << "ERR: No such directory '" << directoryName << "'" << endl;
            return EXIT_FAILURE;
        } else {
            writeAllFiles();
        }
    } else {
        // Print info
        cout << "Possible Arguments:" << endl;
        cout << "  -p path     | Specify the directory path to search in" << endl;
        return 0;
    }
}

bool scanDirectory(const char* dirName, bool recursive, int depth) {
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(dirName)) == NULL) return false;
    while((ent = readdir(dir)) != NULL) {
        if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
        const char* fullPath = (string(dirName)+"\\"+string(ent->d_name)).c_str();
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << ent->d_name << endl;
        if(isHeaderFile(ent->d_name, ent->d_namlen)) {
            activeFilename = string(ent->d_name);
            scanFile(fullPath);
        }
        if (ent->d_type == DT_DIR && recursive)
            scanDirectory(fullPath, recursive, depth+1);
    }
    closedir(dir);
    return true;
}

bool isHeaderFile(const char* fileName, int nameLen) {
    int lastDot = -1;
    for(int i = 0; i < nameLen; i++) {
        if(fileName[i] == '.') lastDot = i;
    }
    if(lastDot < 0) return false;

    int extLen = nameLen-lastDot;
    char extension[extLen];
    for(int i = 0; i < extLen; i++) {
        extension[i] = fileName[lastDot+1+i];
    }

    return ( strcmp(extension, "h") == 0 || strcmp(extension, "hpp") == 0 );
}

void scanFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    activeLineNum = 0;
    while( 1 ) {
        char line[1000];
        fgets(line, 1000, file);
        if(feof(file)) break;

        char header[100];
        if(sscanf(line, "%99s", header));
        if( strcmp(header, "#pragma") == 0 ) {
            parsePragmaLine(line);
        } else if(singleGrab || multiGrab) {
            activeNamespace->addEntry(LineEntry(line), activePriority);
            if(singleGrab) singleGrab = false;
        }

        activeLineNum++;
    }

    singleGrab = false;
    multiGrab = false;
}

void parsePragmaLine(string line) {
    unsigned int indexPound = line.find("#");
    if(indexPound == line.npos) return; // Should never happen
    unsigned int indexSpace = line.find(" ", indexPound+1);
    unsigned int indexLeftParen = line.find("(");
    unsigned int indexRightParen = line.find(")");
    if(indexSpace == line.npos || indexLeftParen == line.npos || indexRightParen == line.npos ||
       indexSpace >= indexLeftParen || indexLeftParen >= indexRightParen) return;

    // Get Command Name
    char cmd_c_str[50];
    sscanf(line.substr(indexSpace+1, indexLeftParen-indexSpace-1).c_str(), " %49s ", cmd_c_str);
    string command(cmd_c_str);

    // Get Arguments
    vector<string> argVec;
    unsigned int lastIndex = indexLeftParen+1;
    while(true) {
        unsigned int indexComma = line.find(",", lastIndex);
        string argSec = "";
        if (indexComma == line.npos || indexComma >= indexRightParen)
            argSec = line.substr(lastIndex, indexRightParen - lastIndex);
        else
            argSec = line.substr(lastIndex, indexComma - lastIndex);
        lastIndex = indexComma + 1;

        // Check for blank Arg
        bool valid = false;
        for (int i = 0; i < argSec.size(); i++) {
            if (argSec[i] != ' ') valid = true;
        }
        if (!valid && (argVec.size() > 0 || !(indexComma == line.npos || indexComma >= indexRightParen))) {
            cerr << "[" << activeFilename << ":" << activeLineNum << "] Empty Parameter! (" << argVec.size() << ")" <<
            endl;
            return;
        }

        // Retrieve Arg without Gaps
        if (valid) {
            char arg_c_str[50];
            sscanf(argSec.c_str(), " %49s ", arg_c_str);
            argVec.push_back(string(arg_c_str));
        }

        // Check for End
        if(indexComma == line.npos || indexComma >= indexRightParen) break;
    }
    int argc = argVec.size();
    string args[argc];
    for(int i = 0; i < argc; i++)
        args[i] = string(argVec[i]);

    parsePragma(command, args, argc);
}

void parsePragma(string command, string args[], int argc) {
    if(command == "lhgSingle" || command == "lhgMultiOn") {

        // Check Arg Count
        if(!(argc == 2 || argc == 3)) {
            cerr << "[" << activeFilename << ":" << activeLineNum << "] ";
            if(command == "lhgMultiOn")
                cerr << "Invalid Number of Parameters for 'lhgMultiOn'; found " << argc << " but requires 2 or 3" << endl;
            else
                cerr << "Invalid Number of Parameters for 'lhgSingle'; found " << argc << " but requires 2 or 3" << endl;
            return;
        }

        // Retrieve Arguments
        string filename = args[0];
        string nmspc = args[1];
        if(nmspc == "LHG_NONE") nmspc = "";
        int priority = 0;
        if(argc == 3) {
            sscanf(args[2].c_str(), "%i", &priority);
            priority *= -1;
        }

        activeNamespace = getFile(filename)->getNamespace(nmspc);
        activePriority = priority;
        if(command == "lhgSingle") singleGrab = true;
        if(command == "lhgMultiOn") multiGrab = true;

    } else if(command == "lhgMultiOff") {

        // Check Arg Count
        if(argc != 0) {
            cerr << "[" << activeFilename << ":" << activeLineNum << "] ";
            cerr << "Invalid Number of Parameters for 'lhgMultiOff'; found " << argc << " but requires 0" << endl;
            return;
        }

        activeNamespace->addEntry(LineEntry("\n"));
        multiGrab = false;

    } else {
        cerr << "[" << activeFilename << ":" << activeLineNum << "] ";
        cerr << "Unknown Custom Pragma Command '" << command << "'" << endl;
        return;
    }
}