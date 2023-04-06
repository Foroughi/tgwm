#include "widget.hpp"
#include "../manager/manager.hpp"

Widget::Widget(std::string name, std::string color, std::string icon, std::function<std::string(Widget *)> updateFunction, std::function<void(int , Manager* mananger)> clickFunction) : Name(name), Color(color), Icon(icon), OnUpdate(updateFunction), OnClick(clickFunction)
{
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
    this->Icon = icon;
}

int Widget::GetInterval()
{
    return this->Interval;
}

void Widget::Update()
{
    this->Value = this->OnUpdate(this);
}

std::string Widget::GetValue()
{
    return this->Value;
}

void Widget::Click(int button ,Manager* manager)
{
    this->OnClick(button , manager);
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