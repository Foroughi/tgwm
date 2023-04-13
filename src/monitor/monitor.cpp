#include "monitor.hpp"
#include <X11/Xutil.h>
#include <glog/logging.h>
#include "../config.hpp"

Monitor::Monitor(Display *display, int screen, Window topbar, std::vector<Tag *> tags) : Topbar(topbar), _Display(display), Screen(screen), Tags(tags)
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
    this->Size.x = x;
    this->Size.y = y;
}

Window Monitor::GetTopbar()
{
    return this->Topbar;
}

Loc Monitor::GetLoc()
{
    return this->Location;
}

void Monitor::SetLoc(int x, int y)
{
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
    this->HideClients(this->SelectedTag->GetIndex());

    this->SelectedTag = this->Tags[index];

    this->ShowClients(this->SelectedTag->GetIndex());

    if (this->OnSelectedTagChanged != NULL)
        this->OnSelectedTagChanged(index);
}

void Monitor::HideClients(int tagIndex)
{
    for (auto it : this->Clients)
    {
        if (it->GetTagIndex() == tagIndex)
            it->Hide();
    }
}

std::vector<Client *> Monitor::GetClients(int tagIndex = -1, FloatingStatus FloatingStatus = FSAll)
{
    // if (tagIndex == -1)
    //     return this->Clients;

    std::vector<Client *> temp = {};
    std::copy_if(this->Clients.begin(), this->Clients.end(), std::back_inserter(temp), [=](Client *c)
                 { 
                    bool flag = true;

                    if(FloatingStatus != FSAll)
                    {
                        if(c->GetFloatStatus() != (FloatingStatus  == FSNormal ? False : True))
                            flag = false;
                    }

                    if(tagIndex != -1)
                    {
                        if(c->GetTagIndex() != tagIndex)
                            flag = false;
                    }

                    return flag; });

    return temp;
}

void Monitor::ShowClients(int tagIndex)
{

    for (auto it : this->Clients)
    {
        if (it->GetTagIndex() == tagIndex)
            it->Show();
    }
}

void Monitor::Sort()
{

    std::vector<Client *> clients = this->GetClients(this->SelectedTag->GetIndex(), FSNormal);

    int cNum = clients.size();

    LOG(INFO) << "Sorting : " << cNum;

    if (cNum == 0)
        return;

    int i = 0;

    if (cNum == 1 || this->_Layout == Layouts_Vertical)
    {

        int w = this->GetSize().x / cNum;
        int y = this->GetSize().y;

        for (auto c : clients)
        {

            c->SetSize(w - (GAP * 2), y - TOP_BAR_HEIGHT - (GAP * 2));
            c->SetLocation((i * w) + GAP + this->GetLoc().x, TOP_BAR_HEIGHT + GAP + this->GetLoc().y);
            i++;

            this->SortDialogs(c);
        }
    }
    else if (this->_Layout == Layouts_Horizontal)
    {
        int w = this->GetSize().x;
        int y = (this->GetSize().y - TOP_BAR_HEIGHT) / cNum;

        for (auto c : clients)
        {
            c->SetSize(w - (GAP * 2), y - (GAP * 2));
            c->SetLocation(GAP + this->GetLoc().x, (y * i) + TOP_BAR_HEIGHT + GAP + this->GetLoc().y);
            i++;

            this->SortDialogs(c);
        }
    }

    else if (this->_Layout == Layouts_Focus)
    {
        int w = this->GetSize().x;
        int y = (this->GetSize().y - TOP_BAR_HEIGHT);

        std::sort(clients.begin(), clients.end(),
                  [](Client *const &a, Client *const &b)
                  {
                      return a->GetPriority() > b->GetPriority();
                  });

        for (auto c : clients)
        {
            if (i == 0)
            {
                c->SetSize(((w * 70) / 100) - (GAP * 2), y - (GAP * 2));
                c->SetLocation(GAP + this->GetLoc().x, TOP_BAR_HEIGHT + GAP + this->GetLoc().y);
            }
            else
            {
                int y2 = (this->GetSize().y - TOP_BAR_HEIGHT) / (cNum - 1);

                c->SetSize(((w * 30) / 100) - (GAP * 2), y2 - (GAP * 2));
                c->SetLocation(((w * 70) / 100) + GAP + this->GetLoc().x, (y2 * (i - 1)) + TOP_BAR_HEIGHT + GAP + this->GetLoc().y);
            }

            i++;

            this->SortDialogs(c);
        }
    }
}

void Monitor::SortDialogs(Client *parent)
{
    std::vector<Client *> clients = this->GetClients(this->SelectedTag->GetIndex(), FSFloating);

    for (auto c : clients)
    {
        if (c->GetParent() == parent)
        {

            int x = parent->GetLocation().x;
            int y = parent->GetLocation().y;

            x += parent->GetSize().x / 2 - (c->GetSize().x / 2);
            y += parent->GetSize().y / 2 - (c->GetSize().y / 2);

            if (x < parent->GetLocation().x)
                x = parent->GetLocation().x;

            if (y < parent->GetLocation().y)
                y = parent->GetLocation().y;

            c->SetLocation(x, y);

            this->SortDialogs(c);
        }
    }
}

void Monitor::AddClient(Display *display, Client *parent, Window frame, Window win, bool isFloating, int tagIndex = 0)
{
    auto c = new Client(display, this->GetLoc(), frame, win, tagIndex);

    c->SetFloatStatus(isFloating);
    c->SetParent(parent);

    if (this->Clients.size() == 0)
        c->SetPriority(1);

    this->Clients.push_back(c);
}

void Monitor::AddClient(Client *c)
{
    c->ChangeMonitor(this->GetLoc());
    this->Clients.push_back(c);
}

void Monitor::SetLayout(Layouts layout)
{
    this->_Layout = layout;
}

Layouts Monitor::GetLayout()
{
    return this->_Layout;
}

void Monitor::RemoveClient(Client *client)
{

    if (!client)
        return;

    client->Hide();

    int i = 0;
    for (auto it : this->Clients)
    {
        if (it == client)
        {
            this->Clients.erase(this->Clients.begin() + i);
            // delete client;
            return;
        }

        i++;
    }
}

Client *Monitor::FindByWinOrFrame(Window win)
{

    for (auto it : this->Clients)
        if (it->GetFrame() == win || it->GetWindow() == win)
            return it;

    return NULL;
}

Client *Monitor::FindByFrame(Window win)
{

    for (auto it : this->Clients)
        if (it->GetFrame() == win)
            return it;

    return NULL;
}

Client *Monitor::FindByWindow(Window win)
{

    for (auto it : this->Clients)
        if (it->GetWindow() == win)
            return it;

    return NULL;
}

void Monitor::SetWidgets(std::vector<Widget *> widgets)
{
    this->Widgets = widgets;
}

std::vector<Widget *> Monitor::GetWidgets()
{
    return this->Widgets;
}

void Monitor::UpdateWidget(std::string name)
{
    for(auto w : this->Widgets)
    {
        if(w->GetName() == name)
            w->SetChangeStatus(true);
    }
}