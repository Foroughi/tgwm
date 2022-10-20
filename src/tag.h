#include <list>
#include <string>

#ifndef __TAG__
#define __TAG__

class Tag
{

private:
    std::string Name;  
    int Index;
public:
    Tag(int Index , std::string name);
    ~Tag();
    std::string GetName();
    int GetIndex();
    
    
};

#endif