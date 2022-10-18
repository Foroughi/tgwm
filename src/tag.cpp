#include "tag.h"

Tag::Tag(std::string name) : Name(name)
{
}

Tag::~Tag()
{
}

std::string Tag::GetName()
{
    return this->Name;
}

void Tag::HideClients()
{

    for (auto it : this->Clients)
    {
        it->Hide();
    }
}
void Tag::ShowClients()
{

    for (auto it : this->Clients)
    {
        it->Show();
    }
}

void Tag::AddClient(Display* display ,  Window win)
{
    this->Clients.push_back(new Client(display , win));
}

void Tag::RemoveClient(Window win){
    //this->Clients.erase(win);
}