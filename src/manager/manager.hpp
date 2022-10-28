#ifndef __MANAGER__
#define __MANAGER__

#include <X11/Xlib.h>
#include <vector>
#include "../monitor/monitor.hpp"
#include "../widget/widget.hpp"

class Manager
{

private:
    Display *CurrentDisplay;
    Window root;
    //std::unordered_map<Window, Window> clients_;
    std::vector<Monitor *> Monitors;
    Monitor* SelectedMonitor;
    Client *SelectedClient;
    bool IsRunning;
    void Config();     
    std::vector<Widget*> Widgets = {};
    bool IsUpdatingWidgets = False;

public:
    Manager(Display *display);
    ~Manager();


    void DrawBars();
    void DrawBar(Monitor *mon);
    void Unframe(Window w);
    void Frame(Window w, bool was_created_before_window_manager);
    void OnConfigureRequest(const XConfigureRequestEvent &e);
    int OnXError(XErrorEvent *e);
    void OnMapRequest(const XMapRequestEvent &e);
    void OnCreateNotify(const XCreateWindowEvent &e);
    void OnDestroyNotify(const XDestroyWindowEvent &e);
    void OnReparentNotify(const XReparentEvent &e);
    void OnMapNotify(const XMapEvent &e);
    void OnUnmapNotify(const XUnmapEvent &e);
    void OnConfigureNotify(const XConfigureEvent &e);
    void OnButtonPress(XButtonPressedEvent &e);
    void reparentAlreadyOpenWindows();
    void OnKeyPress(const XKeyEvent &e);
    void OnMouseEnter(const XCrossingEvent &e);
    void OnMouseLeave(const XCrossingEvent &e);
    Monitor* GetSelectedMonitor();
    Monitor* GetMonitor(int index);
    void Stop();
    int Run();
    Client * FindClientByWin(Window w);
    Monitor* FindMonitorByClient(Client* client);
    static int OnXError(Display *display, XErrorEvent *e);
    void onFocusIn(XFocusChangeEvent &e);
    void OnMotionNotify(XMotionEvent &e);
    Client * GetSelectedClient();
    void MoveSelectedClient(Monitor* mon ,int index);
    void SelectClient(Client *client);
    void DrawWidgets();
    void UpdateWidgets();
    void SortAll();
    void onSelectedTagChanged(int Index);    

};

#endif