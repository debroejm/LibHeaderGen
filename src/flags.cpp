#include "flags.h"

// Actual Flags
unsigned int flags = 0;

bool getFlag(unsigned int flag) {
    return (flag & flags) > 0;
}

void setFlag(unsigned int flag, bool val) {
    if(val) flags = flags | flag;
    else flags = flags & (~flag);
}