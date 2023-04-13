#include "widget.hpp"
#include "../manager/manager.hpp"
#include "../monitor/monitor.hpp"

Widget::Widget(std::string name, std::string color, std::string icon, std::function<std::string(Widget * , Monitor *)> updateFunction, std::function<void(int, Manager *mananger , Widget * widget)> clickFunction) : Name(name), Color(color), Icon(icon), OnUpdate(updateFunction), OnClick(clickFunction)
{
    this->Value = "";
}

Widget::~Widget()
{
}

std::string Widget::GetName()
{
    return this->Name;
}

std::string Widget::GetColor()
{
    return this->Color;
}

std::string Widget::GetIcon()
{
    return this->Icon;
}

void Widget::SetIcon(std::string icon)
{
    if(this->Icon != icon)
    {
        this->Icon = icon;
        this->HasChanged = true;
    }
}

int Widget::GetInterval()
{
    return this->Interval;
}

std::string Widget::Update(Monitor *mon)
{
    try
    {        
        return this->OnUpdate(this , mon);
    }
    catch (const std::exception &e)
    {
        return "ERROR";
    }
}

std::string Widget::GetValue()
{
    return this->Value;
}

void Widget::SetValue(std::string value)
{   
    if(value != this->Value)
    {
        this->Value = value;
        this->HasChanged = true;
    }
}


void Widget::Click(int button, Manager *manager)
{
    this->OnClick(button, manager , this);
}

Rect Widget::GetRect()
{
    return this->MouseRect;
}

void Widget::SetRect(int x, int y, int w, int h)
{
    this->MouseRect = {x, y, w, h};
}

bool Widget::GetHoverStatus()
{
    return this->Hovered;
}
void Widget::SetHoverStatus(bool status)
{
    this->Hovered = status;
}

bool Widget::GetChangeStatus()
{
    return this->HasChanged;
}

void Widget::SetChangeStatus(bool status)
{
    this->HasChanged = status;
}