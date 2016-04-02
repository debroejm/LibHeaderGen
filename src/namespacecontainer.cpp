#include "namespacecontainer.h"

void NamespaceContainer::write(FILE* file, int tabCount) {

    if(name.size() < 1) {
        for(auto &iter : entries) {
            for (int i = 0; i < iter.second.size(); i++) {
                // Write Entry
                writeTabSpacing(file, tabCount);
                fprintf(file, "%s", iter.second[i].getData().c_str());
            }
        }

        for(auto &iter : children) {
            // Have Children Write
            iter.second->write(file, tabCount);
        }
    } else {

        // Write Namespace Header
        writeTabSpacing(file, tabCount);
        fprintf(file, "namespace %s {\n\n", name.c_str());

        for(auto &iter : entries) {
            for (int i = 0; i < iter.second.size(); i++) {
                // Write Entry
                writeTabSpacing(file, tabCount + 1);
                fprintf(file, "%s", iter.second[i].getData().c_str());
            }
        }

        for(auto &iter : children) {
            // Have Children Write
            iter.second->write(file, tabCount + 1);
        }

        // Write Namespace Footer
        writeTabSpacing(file, tabCount);
        fprintf(file, "}\n\n");
    }
}

void NamespaceContainer::addEntry(LineEntry entry, int priority) {
    entries[priority].push_back(entry);
}

void NamespaceContainer::writeTabSpacing(FILE* file, int count) {
    for(int i = 0; i < count; i++) {
        fprintf(file, "    ");
    }
}

NamespaceContainer* NamespaceContainer::getChild(string name) {
    unsigned int indexSep = name.find("::");
    if(indexSep == name.npos) {
        if(children[name] == NULL)
            children[name] = new NamespaceContainer(name);
        return children[name];
    } else {
        string actName = name.substr(0,indexSep);
        string remName = name.substr(indexSep+2,name.npos-indexSep-2);
        if(children[actName] == NULL)
            children[actName] = new NamespaceContainer(actName);
        return children[actName]->getChild(remName);
    }
}



void NamespaceIndex::add(string functionName, NamespaceContainer namespaceName) {
    functionMap[functionName] = namespaceName;
}

NamespaceContainer NamespaceIndex::get(string functionName) {
    return functionMap[functionName];
}