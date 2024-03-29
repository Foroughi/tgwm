#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unordered_map>
#include <glog/logging.h>
#include <sstream>
#include "./manager/manager.hpp"

int main(int argc, char **argv)
{
    ::google::InitGoogleLogging(argv[0]);
       
    Manager* wm = new Manager(XOpenDisplay(NULL));

    #ifdef TGWMDEBUG
    LOG(INFO) << "=======DEBUG MODE========";
    wm->EnableDubugMod();
    #endif
    
    auto res = wm->Run();         

    return res;
}
