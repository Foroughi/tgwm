#include "widget.h"

Widget::Widget(std::string name, std::string color, std::string icon, std::function<std::string(void)> updateFunction) : Name(name), Color(color), Icon(icon), OnUpdate(updateFunction)
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

int Widget::GetInterval()
{
    return this->Interval;
}

std::string Widget::Update()
{
    return this->Value;
}

std::string Widget::GetValue()
{
    return this->OnUpdate();
}
