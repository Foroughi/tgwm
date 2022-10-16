
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <list>
#include <unordered_map>
#include "util.h"

class Client
{

private:
    std::list<Client> SubClients;
    Window Win;    

public:
    Client(Window window);
    ~Client();
};
