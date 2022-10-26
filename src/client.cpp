#include "client.h"

Client::Client(Display *display, Loc monitorLoc, Window frame, Window window, int tagIndex) : MonitorLoc(monitorLoc), Frame(frame), Win(window), _Display(display), TagIndex(tagIndex) {}
Client::~Client() {}

Window Client::GetWindow()
{
    return this->Win;
}

Window Client::GetFrame()
{
    return this->Frame;
}

void Client::Show()
{

    this->IsHidden = False;

    auto h = this->GetSize().y;

    XMoveWindow(this->_Display, this->Frame, 0, h);
    XMoveWindow(this->_Display, this->Win, 0, 0);
}
void Client::Hide()
{
    this->IsHidden = True;

    auto h = this->GetSize().y;

    // this->PreLocation = this->GetLocation();
    // this->SetLocation(width * -4, 0);

    XMoveWindow(this->_Display, this->Frame, 0, h * (-1));
    XMoveWindow(this->_Display, this->Win, 0, 0);
}

bool Client::GetVisibilityStatus()
{

    return this->IsHidden;
}

Loc Client::GetSize()
{

    static XWindowAttributes wa;
    !XGetWindowAttributes(this->_Display, this->Frame, &wa);

    Loc l;
    l.x = wa.width;
    l.y = wa.height;

    return l;
}

void Client::SetSize(int x, int y)
{
    XResizeWindow(this->_Display, this->Frame, x - (BORDER_WIDTH * 2), y - (BORDER_WIDTH * 2));
    XResizeWindow(this->_Display, this->Win, x - (BORDER_WIDTH * 2), y - (BORDER_WIDTH * 2));
}

Loc Client::GetLocation()
{

    static XWindowAttributes wa;
    !XGetWindowAttributes(this->_Display, this->Frame, &wa);

    Loc l;
    l.x = wa.x;
    l.y = wa.y;

    return l;
}

void Client::SetLocation(int x, int y)
{

    XMoveWindow(this->_Display, this->Frame, x, y);
    XMoveWindow(this->_Display, this->Win, 0, 0);
}

int Client::GetTagIndex()
{
    return this->TagIndex;
}

void Client::SetTagIndex(int index)
{
    this->TagIndex = index;
}

void Client::ChangeMonitor(Loc monitorLoc)
{
    this->MonitorLoc = monitorLoc;
}