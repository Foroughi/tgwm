
#ifndef __WIDGET__
#define __WIDGET__
#include "../util/util.hpp"
#include <list>
#include <string>
#include <functional>

class Manager;
class Monitor;
class Widget
{

private:
    std::string Name;
    std::string Color;
    std::string Icon;
    int Interval;
    std::string Value = "";
    bool Hovered = False;
    Rect MouseRect;
    bool HasChanged = True;

public:
    Widget(std::string name, std::string colo, std::string icon, std::function<std::string(Widget * , Monitor *)> updateFunction, std::function<void(int, Manager * , Widget *)> clickFunction);
    ~Widget();
    std::string GetName();
    std::string GetColor();
    void SetIcon(std::string icon);
    std::string GetIcon();
    int GetInterval();
    std::string Update(Monitor *mon);
    std::string GetValue();
    void SetValue(std::string value);
    void Click(int button, Manager *mananger);
    std::function<std::string(Widget * , Monitor *)> OnUpdate;
    std::function<void(int, Manager * , Widget *)> OnClick;
    Rect GetRect();
    void SetRect(int x, int y, int w, int h);
    bool GetChangeStatus();
    void SetChangeStatus(bool status);
    bool GetHoverStatus();    
    void SetHoverStatus(bool status);
};

#endif