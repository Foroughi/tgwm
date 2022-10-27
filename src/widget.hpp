
#ifndef __WIDGET__
#define __WIDGET__

#include <list>
#include <string>
#include <functional>

class Widget
{

private:
    std::string Name;
    std::string Color;
    std::string Icon;
    int Interval;
    std::string Value = "";

public:
    Widget(std::string name, std::string colo, std::string icon, std::function<std::string(Widget *)> updateFunction, std::function<void(int)> clickFunction);
    ~Widget();
    std::string GetName();
    std::string GetColor();
    void SetIcon(std::string icon);
    std::string GetIcon();
    int GetInterval();
    void Update();
    std::string GetValue();
    void Click(int button);
    std::function<std::string(Widget *)> OnUpdate;
    std::function<void(int)> OnClick;
};

#endif