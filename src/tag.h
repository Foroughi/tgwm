#include <list>
#include "client.h"
#include <string>

#ifndef __TAG__
#define __TAG__

class Tag
{

private:
    std::string Name;
    std::list<Client*> Clients;

public:
    Tag(std::string name);
    ~Tag();
    std::string GetName();

    void HideClients();
    void ShowClients();
    void AddClient(Display* display ,  Window win);
    void RemoveClient(Window win);
};

#endif