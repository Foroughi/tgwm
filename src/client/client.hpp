
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

public:
    Client(Display *display, Loc monitorLoc, Window Frame, Window window, int TagIndex);
    ~Client();

    void Show();
    void Hide();

    bool GetVisibilityStatus();

    Loc GetSize();
    void SetSize(int x, int y);

    Window GetWindow();
    Window GetFrame();

    Loc GetLocation();
    void SetLocation(int x, int y);

    int GetTagIndex();
    void SetTagIndex(int index);

    bool GetFloatStatus();
    void SetFloatStatus(bool status);

    Client* GetParent();
    void SetParent(Client* Parent);

    void ChangeMonitor(Loc monitorLoc);
};

#endif