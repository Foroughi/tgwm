
#include "tag.hpp"

Tag::Tag(int index, std::string name, std::string icon) : Name(name), Index(index), Icon(icon)
{
}

Tag::~Tag()
{
}

std::string Tag::GetName()
{
    return this->Name;
}

std::string Tag::GetIcon()
{
    return this->Icon;
}

int Tag::GetIndex()
{
    return this->Index;
}

Rect Tag::GetRect()
{
    return this->MouseRect;
}

void Tag::SetRect(int x, int y, int w, int h)
{
    this->MouseRect = {x, y, w, h};
}

bool Tag::GetHoverStatus()
{
    return this->Hovered;
}
void Tag::SetHoverStatus(bool status)
{
    this->Hovered = status;
}