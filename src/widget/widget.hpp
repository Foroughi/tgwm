
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
    std::vector<bool> MonitorDisplayStatus;

public:
    Widget(std::string name, std::string colo, std::string icon, std::vector<bool> MonitorDisplayStatus, std::function<std::string(Widget * , Monitor *)> updateFunction, std::function<void(int, Manager *)> clickFunction);
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
    std::function<void(int, Manager *)> OnClick;
    Rect GetRect();
    void SetRect(int x, int y, int w, int h);
    bool GetHoverStatus();
    std::vector<bool> GetMonitorDisplayStatus();
    void SetHoverStatus(bool status);
};

#endif