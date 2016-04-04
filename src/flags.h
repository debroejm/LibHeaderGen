#pragma once

#define FLAG_GRAB_SINGLE 1
#define FLAG_GRAB_MULTI 2
#define FLAG_PROCESS_DEFINES 4

#define FLAG_ALL 7

void setFlag(unsigned int flag, bool val);
bool getFlag(unsigned int flag);