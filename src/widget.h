#include <list>
#include <string>
#include <functional>

#ifndef __WIDGET__
#define __WIDGET__

class Widget
{

private:

    std::string Name;
    std::string Color;
    std::string Icon;    
    int Interval;
    std::string Value = "";

public:

    Widget(std::string name , std::string colo , std::string icon , std::function<std::string(void)> updateFunction);
    ~Widget();
    std::string GetName();
    std::string GetColor();
    std::string GetIcon();
    int GetInterval();
    std::string Update();
    std::string GetValue();
    std::function<std::string(void)> OnUpdate;
    
};

#endif