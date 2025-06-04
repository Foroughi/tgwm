#include "../client/client.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>
#include "../util/util.hpp"
#include "../config.hpp"
#include <unistd.h>

Client::Client(Display *display, Loc monitorLoc, Window frame, Window window, int tagIndex) : MonitorLoc(monitorLoc), Frame(frame), Win(window), _Display(display), TagIndex(tagIndex) {}
Client::~Client()
{

}

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

    //auto h = this->GetSize().y;
    XMapWindow(this->_Display, this->Frame);
    XFlush(this->_Display);
    //XMoveWindow(this->_Display, this->Frame, 0, h);
    //XMoveWindow(this->_Display, this->Win, 0, 0);
}
void Client::Hide()
{
    this->IsHidden = True;

    auto h = this->GetSize().y;

    // this->PreLocation = this->GetLocation();
    // this->SetLocation(width * -4, 0);

    //XMoveWindow(this->_Display, this->Frame, 0, h * (-1));
    //XMoveWindow(this->_Display, this->Win, 0, 0);
    XUnmapWindow(this->_Display , this->Frame);
    XFlush(this->_Display);
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

Loc Client::GetWinSize()
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
    int steps = 20;   // Number of steps for smooth resizing
    int delay = 7;
    static XWindowAttributes wa;
    !XGetWindowAttributes(this->_Display, this->Frame, &wa);

    int endW = x - (BORDER_WIDTH * 2);
    int endH = y - (BORDER_WIDTH * 2);
    int startW = wa.width;
    int startH = wa.height;
    int deltaW = (endW - wa.width) / steps;
    int deltaH = (endH - wa.height) / steps;

    if(ANIMATION && wa.x > 0 && wa.y > 0)
    {
        for (int i = 0; i < steps; i++) {
            startW += deltaW;
            startH += deltaH;
            XResizeWindow(this->_Display, this->Frame, startW, startH);
            XResizeWindow(this->_Display, this->Win, startW, startH);
            XFlush(this->_Display);  // Send updates to the X server
            usleep(delay * 1000);  // Delay in milliseconds
        }
    }
    XResizeWindow(this->_Display, this->Frame, endW , endH);
    XResizeWindow(this->_Display, this->Win, endW,endH);
    XFlush(this->_Display);
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

Loc Client::GetWinLocation()
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

    int steps = 20;   // Number of steps for smooth resizing
    int delay = 7;
    static XWindowAttributes wa;
    !XGetWindowAttributes(this->_Display, this->Frame, &wa);
    if(ANIMATION && wa.x > 0 && wa.y > 0)
    {
        int endW = x;
        int endH = y;
        int startW = wa.x;
        int startH = wa.y;
        int deltaW = (endW - startW) / steps;
        int deltaH = (endH - startH) / steps;

        for (int i = 0; i < steps; i++) {
            startW += deltaW;
            startH += deltaH;
            XMoveWindow(this->_Display, this->Frame, startW, startH);
            //XResizeWindow(this->_Display, this->Win, startW, startH);
            XFlush(this->_Display);  // Send updates to the X server
            usleep(delay * 1000);  // Delay in milliseconds

        }
    }
    XMoveWindow(this->_Display, this->Frame, x, y);
    XMoveWindow(this->_Display, this->Win, 0, 0);
    XFlush(this->_Display);  // Send updates to the X server
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

bool Client::GetFloatStatus()
{
    return this->Floating;
}

void Client::SetFloatStatus(bool status)
{
    this->Floating = status;
}

bool Client::GetFullscreen()
{
    return this->isFullscreen;
}

void Client::SetFullscreen(bool status)
{
    this->isFullscreen = status;


}

Client *Client::GetParent()
{
    return this->Parent;
}

void Client::SetParent(Client *parent)
{
    this->Parent = parent;
}

int Client::GetPriority()
{
    return this->priority;
}

void Client::SetPriority(int priority)
{
    this->priority = priority;
}
