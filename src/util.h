#include <X11/Xutil.h>
#include <unordered_map>
#include <glog/logging.h>

#ifndef __UTIL__
#define __UTIL__
struct Loc{
    int x;
    int y;
};
void start(char *what);
std::string ToString(const XEvent &e);

#endif