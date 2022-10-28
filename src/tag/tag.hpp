#ifndef __TAG__
#define __TAG__

#include "../util/util.hpp"
#include <string>
#include <functional>

class Tag
{

private:
    std::string Name;
    std::string Icon;
    Rect MouseRect;
    bool Hovered = False;
    int Index;

public:
    Tag(int Index, std::string name, std::string icon);
    ~Tag();
    std::string GetName();
    std::string GetIcon();
    int GetIndex();

    Rect GetRect();
    void SetRect(int x, int y, int w, int h);

    bool GetHoverStatus();
    void SetHoverStatus(bool status);    
};

#endif