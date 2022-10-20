#include "tag.h"

Tag::Tag(int index , std::string name) : Name(name) , Index(index)
{
}

Tag::~Tag()
{
}

std::string Tag::GetName()
{
    return this->Name;
}

int Tag::GetIndex()
{
    return this->Index;
}