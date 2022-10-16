
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <glog/logging.h>
#include <string>
#include <list>
#include <unordered_map>
#include "monitor.h"

#ifndef __WINDOWMANAGER__
#define __WINDOWMANAGER__

class WindowManager
{

private:
    Display *CurrentDisplay;
    Window root;
    std::unordered_map<Window, Window> clients_;
    std::list<Monitor *> Monitors;
    bool IsRunning;
    void Config();

public:
    WindowManager(Display *display);
    ~WindowManager();
    void DrawBars();
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
    void Run();
    static int OnXError(Display *display, XErrorEvent *e);
};

#endif