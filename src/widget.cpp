#include "widget.hpp"

Widget::Widget(std::string name, std::string color, std::string icon, std::function<std::string(Widget *)> updateFunction, std::function<void(int)> clickFunction) : Name(name), Color(color), Icon(icon), OnUpdate(updateFunction), OnClick(clickFunction)
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

void Widget::Click(int button)
{
    this->OnClick(button);
}
