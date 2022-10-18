#include "client.h"

Client::Client(Display *display, Window window) : Win(window), _Display(display) {}
Client::~Client() {}

void Client::Show()
{

    this->IsHidden = False;

    this->SetLocation(this->PreLocation.x, this->PreLocation.y);
}
void Client::Hide()
{
    this->IsHidden = True;

    auto width = this->GetSize().x;

    this->PreLocation = this->GetLocation();
    this->SetLocation(width * -2, 0);
}

bool Client::GetVisibilityStatus()
{

    return this->IsHidden;
}

Loc Client::GetSize()
{

    static XWindowAttributes wa;
    !XGetWindowAttributes(this->_Display, this->Win, &wa);

    Loc l;
    l.x = wa.width;
    l.y = wa.height;

    return l;
}

void Client::SetSize(int x, int y)
{
    XResizeWindow(this->_Display, this->Win, x, y);
}

Loc Client::GetLocation()
{

    static XWindowAttributes wa;
    !XGetWindowAttributes(this->_Display, this->Win, &wa);

    Loc l;
    l.x = wa.x;
    l.y = wa.y;

    return l;
}

void Client::SetLocation(int x, int y)
{
    XMoveWindow(this->_Display, this->Win, x, y);
}