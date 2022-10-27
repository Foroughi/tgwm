#ifndef __TAG__
#define __TAG__


#include <string>

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