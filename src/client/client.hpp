
#ifndef __CLIENT__
#define __CLIENT__

#include <X11/Xlib.h>
#include "../util/util.hpp"

class Client
{

private:
    Window Win;
    Window Frame;
    bool IsHidden = False;
    Display *_Display;
    Loc MonitorLoc;   
    int TagIndex;
    bool Floating = False;
    Client* Parent = NULL;
    int priority = 0;

public:
    Client(Display *display, Loc monitorLoc, Window Frame, Window window, int TagIndex);
    ~Client();

    void Show();
    void Hide();

    bool GetVisibilityStatus();

    Loc GetSize();
    Loc GetWinSize();
    void SetSize(int x, int y);

    Window GetWindow();
    Window GetFrame();

    Loc GetLocation();
    Loc GetWinLocation();
    void SetLocation(int x, int y);

    int GetTagIndex();
    void SetTagIndex(int index);

    int GetPriority();
    void SetPriority(int priority);

    bool GetFloatStatus();
    void SetFloatStatus(bool status);

    Client* GetParent();
    void SetParent(Client* Parent);

    void ChangeMonitor(Loc monitorLoc);
};

#endif