#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unordered_map>
#include <glog/logging.h>
#include <sstream>
#include "manager.h"

int main(int argc, char **argv)
{
    ::google::InitGoogleLogging(argv[0]);
       
    Manager* wm = new Manager(XOpenDisplay(NULL));

    wm->Run();
    
    return EXIT_SUCCESS;
}
