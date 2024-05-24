#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unordered_map>
#include <sstream>
#include "./manager/manager.hpp"

int main(int argc, char **argv)
{
           
    Manager* wm = new Manager(XOpenDisplay(NULL));

    #ifdef TGWMDEBUG
    wm->EnableDubugMod();
    #endif
    
    auto res = wm->Run();         

    return res;
}
