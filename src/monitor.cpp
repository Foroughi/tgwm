#include "monitor.h"

Monitor::Monitor(Display *display, int screen, std::vector<Tag *> tags) : _Display(display), Screen(screen), Tags(tags)
{

    this->SelectedTag = this->Tags.at(0);
}

Monitor::~Monitor() {}

void Monitor::setTags(std::vector<Tag *> tags)
{
    this->Tags = tags;
}

std::vector<Tag *> Monitor::GetTags()
{
    return this->Tags;
}

Loc Monitor::GetSize()
{
    return this->Size;
}

void Monitor::SetSize(int x, int y)
{
    this->Size;
    this->Size.x = x;
    this->Size.y = y;
}

Loc Monitor::GetLoc()
{
    return this->Location;
}

void Monitor::SetLoc(int x, int y)
{
    this->Location;
    this->Location.x = x;
    this->Location.y = y;
}

int Monitor::GetScreen()
{
    return this->Screen;
}

Tag *Monitor::GetSelectedTag()
{
    return this->SelectedTag;
}

void Monitor::SelectTagByIndex(int index)
{
    this->SelectedTag->HideClients();

    this->SelectedTag = this->Tags[index];

    this->SelectedTag->ShowClients();

    if (this->OnSelectedTagChanged != NULL)
        this->OnSelectedTagChanged(index);
}