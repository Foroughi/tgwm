
#ifndef __WIDGET__
#define __WIDGET__
#include "../util/util.hpp"
#include <list>
#include <string>
#include <functional>


class Manager;
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

public:
    Widget(std::string name, std::string colo, std::string icon, std::function<std::string(Widget *)> updateFunction, std::function<void(int , Manager*)> clickFunction);
    ~Widget();
    std::string GetName();
    std::string GetColor();
    void SetIcon(std::string icon);
    std::string GetIcon();
    int GetInterval();
    void Update();
    std::string GetValue();
    void Click(int button, Manager* mananger);
    std::function<std::string(Widget *)> OnUpdate;
    std::function<void(int, Manager*)> OnClick;    
    Rect GetRect();
    void SetRect(int x, int y, int w, int h);

    bool GetHoverStatus();
    void SetHoverStatus(bool status);
};

#endif