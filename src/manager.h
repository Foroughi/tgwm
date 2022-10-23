
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <glog/logging.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "monitor.h"
#ifdef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif /* XINERAMA */

#ifndef __MANAGER__
#define __MANAGER__

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
    void reparentAlreadyOpenWindows();
    void OnKeyPress(const XKeyEvent &e);
    void OnMouseEnter(const XCrossingEvent &e);
    void OnMouseLeave(const XCrossingEvent &e);
    int Run();
    Client * FindClientByWin(Window w);
    Monitor* FindMonitorByClient(Client* client);
    static int OnXError(Display *display, XErrorEvent *e);
    void onFocusIn(XFocusChangeEvent &e);
    void OnMotionNotify(XMotionEvent &e);
    Client * GetSelectedClient();
    void MoveSelectedClient(Monitor* mon ,int index);
    void SelectClient(Client *client);
    void SortAll();
    void onSelectedTagChanged(int Index);
};

#endif