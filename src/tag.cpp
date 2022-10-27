#include "tag.hpp"

Tag::Tag(int index , std::string name , std::string icon) : Name(name) , Index(index) , Icon(icon)
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