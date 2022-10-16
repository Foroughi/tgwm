#include <list>
#include "client.h"
#include <string>

#ifndef __TAG__
#define __TAG__

class Tag
{

private:
    std::string Name;
    std::list<Client> Clients;

public:
    Tag(std::string name);
    ~Tag();
};

#endif