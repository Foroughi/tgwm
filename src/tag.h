#include <list>
#include <string>

#ifndef __TAG__
#define __TAG__

class Tag
{

private:
    std::string Name;  
    std::string Icon;
    int Index;
public:
    Tag(int Index , std::string name , std::string icon);
    ~Tag();
    std::string GetName();
    std::string GetIcon();
    int GetIndex();
    
    
};

#endif