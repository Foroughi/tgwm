#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unordered_map>
#include <glog/logging.h>
#include <sstream>

using ::std::string;

::std::unordered_map<Window, Window> clients_;
bool IsRunning = True;

void start(char *what)
{
    if (!fork())
    {
        char *const args[] = {"/bin/sh", "-c", what, NULL};
        execvp("/bin/sh", args);
        exit(1);
    }
}

string ToString(const XEvent &e)
{
    static const char *const X_EVENT_TYPE_NAMES[] = {
        "",
        "",
        "KeyPress",
        "KeyRelease",
        "ButtonPress",
        "ButtonRelease",
        "MotionNotify",
        "EnterNotify",
        "LeaveNotify",
        "FocusIn",
        "FocusOut",
        "KeymapNotify",
        "Expose",
        "GraphicsExpose",
        "NoExpose",
        "VisibilityNotify",
        "CreateNotify",
        "DestroyNotify",
        "UnmapNotify",
        "MapNotify",
        "MapRequest",
        "ReparentNotify",
        "ConfigureNotify",
        "ConfigureRequest",
        "GravityNotify",
        "ResizeRequest",
        "CirculateNotify",
        "CirculateRequest",
        "PropertyNotify",
        "SelectionClear",
        "SelectionRequest",
        "SelectionNotify",
        "ColormapNotify",
        "ClientMessage",
        "MappingNotify",
        "GeneralEvent",
    };

    return X_EVENT_TYPE_NAMES[e.type];
}

void Unframe(Display *display_, Window root_, Window w)
{

    const Window frame = clients_[w];
    XUnmapWindow(display_, frame);
    XReparentWindow(display_, w, root_, 0, 0);
    XRemoveFromSaveSet(display_, w);
    XDestroyWindow(display_, frame);
    clients_.erase(w);

    LOG(INFO) << "Unframed window " << w << " [" << frame << "]";
}

void Frame(Display *display_, Window root_, Window w, bool was_created_before_window_manager)
{

    const unsigned int BORDER_WIDTH = 3;
    const unsigned long BORDER_COLOR = 0xff0000;
    const unsigned long BG_COLOR = 0x0000ff;

    CHECK(!clients_.count(w));

    XWindowAttributes x_window_attrs;
    CHECK(XGetWindowAttributes(display_, w, &x_window_attrs));

    if (was_created_before_window_manager)
    {
        if (x_window_attrs.override_redirect ||
            x_window_attrs.map_state != IsViewable)
        {
            return;
        }
    }

    const Window frame = XCreateSimpleWindow(display_, root_, 50, 50, x_window_attrs.width, x_window_attrs.height, BORDER_WIDTH, BORDER_COLOR, BG_COLOR);

    // XSelectInput(display_, frame, EnterWindowMask | FocusChangeMask | PropertyChangeMask | StructureNotifyMask);
    XSelectInput(
        display_,
        frame,
        SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask);
    XAddToSaveSet(display_, w);
    XReparentWindow(display_, w, frame, 0, 0);
    XMapWindow(display_, frame);
    clients_[w] = frame;

    // XGrabButton(display_, Button1, Mod1Mask, w, false, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    // XGrabButton(display_, Button3, Mod1Mask, w, false, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    // XGrabKey(display_, XKeysymToKeycode(display_, XK_F4), Mod1Mask, w, false, GrabModeAsync, GrabModeAsync);
    // XGrabKey(display_, XKeysymToKeycode(display_, XK_Tab), Mod1Mask, w, false, GrabModeAsync, GrabModeAsync);

    LOG(INFO) << "Framed window " << w << " [" << frame << "]";
}

void OnConfigureRequest(Display *display_, const XConfigureRequestEvent &e)
{
    XWindowChanges changes;
    changes.x = e.x;
    changes.y = e.y;
    changes.width = e.width;
    changes.height = e.height;
    changes.border_width = e.border_width;
    changes.sibling = e.above;
    changes.stack_mode = e.detail;
    if (clients_.count(e.window))
    {
        const Window frame = clients_[e.window];
        XConfigureWindow(display_, frame, e.value_mask, &changes);
        LOG(INFO) << "Resize [" << frame << "] to " << e.width << " " << e.height;
    }
    XConfigureWindow(display_, e.window, e.value_mask, &changes);
    LOG(INFO) << "Resize " << e.window << " to " << e.width << " " << e.height;
}

int OnXError(Display *display, XErrorEvent *e)
{
    
    const int MAX_ERROR_TEXT_LENGTH = 1024;
    char error_text[MAX_ERROR_TEXT_LENGTH];
    XGetErrorText(display, e->error_code, error_text, sizeof(error_text));

    return 0;
}

void OnMapRequest(Display *display_, Window root_, const XMapRequestEvent &e)
{    
    Frame(display_, root_, e.window, false);    
    XMapWindow(display_, e.window);
}

void OnCreateNotify(const XCreateWindowEvent &e) {}

void OnDestroyNotify(const XDestroyWindowEvent &e) {}

void OnReparentNotify(const XReparentEvent &e) {}

void OnMapNotify(const XMapEvent &e) {}

void OnUnmapNotify(Display *display_, Window root_, const XUnmapEvent &e)
{
    if (!clients_.count(e.window))
    {
        LOG(INFO) << "Ignore UnmapNotify for non-client window " << e.window;
        return;
    }
    if (e.event == root_)
    {
        LOG(INFO) << "Ignore UnmapNotify for reparented pre-existing window "
                  << e.window;
        return;
    }

    Unframe(display_, root_, e.window);
}

void OnConfigureNotify(const XConfigureEvent &e) {}

void reparentAlreadyOpenWindows(Display *display_, Window root_)
{
    XGrabServer(display_);
    Window returned_root, returned_parent;
    Window *top_level_windows;
    unsigned int num_top_level_windows;
    CHECK(XQueryTree(
        display_,
        root_,
        &returned_root,
        &returned_parent,
        &top_level_windows,
        &num_top_level_windows));
    CHECK_EQ(returned_root, root_);
    for (unsigned int i = 0; i < num_top_level_windows; ++i)
    {
        Frame(display_, root_, top_level_windows[i], true);
    }
    XFree(top_level_windows);
    XUngrabServer(display_);
}

void OnKeyPress(Display *display, Window root_, const XKeyEvent &e)
{

    if (e.keycode == XKeysymToKeycode(display, XStringToKeysym("F1")))
    {
        IsRunning = False;
    }
    else if (e.keycode == XKeysymToKeycode(display, XStringToKeysym("F2")))
    {
        start("xterm");

        // reparentAlreadyOpenWindows(display , root_);
    }
}

void Run(Display *display_, Window root_)
{

    XSelectInput(
        display_,
        root_,
        KeyPressMask | SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask | PropertyChangeMask);
    XSync(display_, false);

    XSetErrorHandler(OnXError);
    //   c. Grab X server to prevent windows from changing under us.
    XGrabServer(display_);
    //   d. Reparent existing top-level windows.
    //     i. Query existing top-level windows.
    Window returned_root, returned_parent;
    Window *top_level_windows;
    unsigned int num_top_level_windows;
    CHECK(XQueryTree(
        display_,
        root_,
        &returned_root,
        &returned_parent,
        &top_level_windows,
        &num_top_level_windows));
    CHECK_EQ(returned_root, root_);
    //     ii. Frame each top-level window.
    for (unsigned int i = 0; i < num_top_level_windows; ++i)
    {
        Frame(display_, root_, top_level_windows[i], true);
    }
    //     iii. Free top-level window array.
    XFree(top_level_windows);
    //   e. Ungrab X server.
    XUngrabServer(display_);

    XGrabKey(display_, XKeysymToKeycode(display_, XStringToKeysym("F1")), Mod1Mask, root_, False, GrabModeAsync, GrabModeAsync);

    // 2. Main event loop.
    while (IsRunning)
    {
        // 1. Get next event.
        XEvent e;
        XNextEvent(display_, &e);
        LOG(INFO) << "Received event: " << ToString(e);

        // 2. Dispatch event.
        switch (e.type)
        {
        case CreateNotify:
            OnCreateNotify(e.xcreatewindow);
            break;
        case DestroyNotify:
            OnDestroyNotify(e.xdestroywindow);
            break;
        case ReparentNotify:
            OnReparentNotify(e.xreparent);
            break;
        case MapNotify:
            OnMapNotify(e.xmap);
            break;
        case UnmapNotify:
            OnUnmapNotify(display_, root_, e.xunmap);
            break;
        case ConfigureNotify:
            OnConfigureNotify(e.xconfigure);
            break;
        case MapRequest:
            OnMapRequest(display_, root_, e.xmaprequest);
            break;
        case ConfigureRequest:
            OnConfigureRequest(display_, e.xconfigurerequest);
            break;
        case ButtonPress:
            // OnButtonPress(e.xbutton);
            break;
        case ButtonRelease:
            // OnButtonRelease(e.xbutton);
            break;
        case MotionNotify:
            // Skip any already pending motion events.
            // while (XCheckTypedWindowEvent(
            //     display_, e.xmotion.window, MotionNotify, &e))
            // {
            // }
            // OnMotionNotify(e.xmotion);
            break;
        case KeyPress:
            OnKeyPress(display_, root_, e.xkey);
            break;
        case KeyRelease:
            // OnKeyRelease(e.xkey);
            break;
        default:
            LOG(WARNING) << "Ignored event";
        }
    }
}

int main(int argc, char **argv)
{
    ::google::InitGoogleLogging(argv[0]);

    LOG(INFO) << "Starting App";

    auto display_ = XOpenDisplay(NULL);
    auto root_ = DefaultRootWindow(display_);

    Run(display_, root_);

    XCloseDisplay(display_);

    return EXIT_SUCCESS;
}
