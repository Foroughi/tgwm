#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <glog/logging.h>
#include <unordered_map>
#include "util.h"

class WindowManager
{

private:
    Display *CurrentDisplay;
    Window root;
    std::unordered_map<Window, Window> clients_;
    bool IsRunning;

public:
    WindowManager(Display *display) : CurrentDisplay(display), root(DefaultRootWindow(display))
    {

        this->IsRunning = True;
    }

    ~WindowManager()
    {
        XCloseDisplay(this->CurrentDisplay);
    }

    void Unframe(Window w)
    {

        const Window frame = clients_[w];
        XUnmapWindow(this->CurrentDisplay, frame);
        XReparentWindow(this->CurrentDisplay, w, this->root, 0, 0);
        XRemoveFromSaveSet(this->CurrentDisplay, w);
        XDestroyWindow(this->CurrentDisplay, frame);
        clients_.erase(w);

        LOG(INFO) << "Unframed window " << w << " [" << frame << "]";
    }

    void Frame(Window w, bool was_created_before_window_manager)
    {

        const unsigned int BORDER_WIDTH = 3;
        const unsigned long BORDER_COLOR = 0xff0000;
        const unsigned long BG_COLOR = 0x0000ff;

        CHECK(!clients_.count(w));

        XWindowAttributes x_window_attrs;
        CHECK(XGetWindowAttributes(this->CurrentDisplay, w, &x_window_attrs));

        if (was_created_before_window_manager)
        {
            if (x_window_attrs.override_redirect ||
                x_window_attrs.map_state != IsViewable)
            {
                return;
            }
        }

        const Window frame = XCreateSimpleWindow(this->CurrentDisplay, this->root, 50, 50, x_window_attrs.width, x_window_attrs.height, BORDER_WIDTH, BORDER_COLOR, BG_COLOR);

        // XSelectInput(this->CurrentDisplay, frame, EnterWindowMask | FocusChangeMask | PropertyChangeMask | StructureNotifyMask);
        XSelectInput(
            this->CurrentDisplay,
            frame,
            SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask);
        XAddToSaveSet(this->CurrentDisplay, w);
        XReparentWindow(this->CurrentDisplay, w, frame, 0, 0);
        XMapWindow(this->CurrentDisplay, frame);
        clients_[w] = frame;

        // XGrabButton(this->CurrentDisplay, Button1, Mod1Mask, w, false, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask, GrabModeAsync, GrabModeAsync, None, None);
        // XGrabButton(this->CurrentDisplay, Button3, Mod1Mask, w, false, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask, GrabModeAsync, GrabModeAsync, None, None);
        // XGrabKey(this->CurrentDisplay, XKeysymToKeycode(this->CurrentDisplay, XK_F4), Mod1Mask, w, false, GrabModeAsync, GrabModeAsync);
        // XGrabKey(this->CurrentDisplay, XKeysymToKeycode(this->CurrentDisplay, XK_Tab), Mod1Mask, w, false, GrabModeAsync, GrabModeAsync);

        LOG(INFO) << "Framed window " << w << " [" << frame << "]";
    }

    void OnConfigureRequest(const XConfigureRequestEvent &e)
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
            XConfigureWindow(this->CurrentDisplay, frame, e.value_mask, &changes);
            LOG(INFO) << "Resize [" << frame << "] to " << e.width << " " << e.height;
        }
        XConfigureWindow(this->CurrentDisplay, e.window, e.value_mask, &changes);
        LOG(INFO) << "Resize " << e.window << " to " << e.width << " " << e.height;
    }

    int OnXError(XErrorEvent *e)
    {

        const int MAX_ERROR_TEXT_LENGTH = 1024;
        char error_text[MAX_ERROR_TEXT_LENGTH];
        XGetErrorText(this->CurrentDisplay, e->error_code, error_text, sizeof(error_text));

        return 0;
    }

    void OnMapRequest(const XMapRequestEvent &e)
    {
        Frame(e.window, false);
        XMapWindow(this->CurrentDisplay, e.window);
    }

    void OnCreateNotify(const XCreateWindowEvent &e) {}

    void OnDestroyNotify(const XDestroyWindowEvent &e) {}

    void OnReparentNotify(const XReparentEvent &e) {}

    void OnMapNotify(const XMapEvent &e) {}

    void OnUnmapNotify(const XUnmapEvent &e)
    {
        if (!clients_.count(e.window))
        {
            LOG(INFO) << "Ignore UnmapNotify for non-client window " << e.window;
            return;
        }
        if (e.event == this->root)
        {
            LOG(INFO) << "Ignore UnmapNotify for reparented pre-existing window "
                      << e.window;
            return;
        }

        Unframe(e.window);
    }

    void OnConfigureNotify(const XConfigureEvent &e) {}

    void reparentAlreadyOpenWindows()
    {
        XGrabServer(this->CurrentDisplay);
        Window returned_root, returned_parent;
        Window *top_level_windows;
        unsigned int num_top_level_windows;
        CHECK(XQueryTree(
            this->CurrentDisplay,
            this->root,
            &returned_root,
            &returned_parent,
            &top_level_windows,
            &num_top_level_windows));
        CHECK_EQ(returned_root, this->root);
        for (unsigned int i = 0; i < num_top_level_windows; ++i)
        {
            Frame(top_level_windows[i], true);
        }
        XFree(top_level_windows);
        XUngrabServer(this->CurrentDisplay);
    }

    void OnKeyPress(const XKeyEvent &e)
    {

        if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("F1")))
        {
            IsRunning = False;
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("F2")))
        {
            start("xterm");

            // reparentAlreadyOpenWindows(display , this->root);
        }
    }

    void Run()
    {

        XSelectInput(
            this->CurrentDisplay,
            this->root,
            KeyPressMask | SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask | PropertyChangeMask);
        XSync(this->CurrentDisplay, false);

        XSetErrorHandler(OnXError);
        XGrabServer(this->CurrentDisplay);

        Window returned_root, returned_parent;
        Window *top_level_windows;
        unsigned int num_top_level_windows;
        CHECK(XQueryTree(
            this->CurrentDisplay,
            this->root,
            &returned_root,
            &returned_parent,
            &top_level_windows,
            &num_top_level_windows));
        CHECK_EQ(returned_root, this->root);

        for (unsigned int i = 0; i < num_top_level_windows; ++i)
        {
            Frame(top_level_windows[i], true);
        }

        XFree(top_level_windows);

        XUngrabServer(this->CurrentDisplay);

        XGrabKey(this->CurrentDisplay, XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("F1")), Mod1Mask, this->root, False, GrabModeAsync, GrabModeAsync);

        while (IsRunning)
        {

            XEvent e;
            XNextEvent(this->CurrentDisplay, &e);
            LOG(INFO) << "Received event: " << ToString(e);

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
                OnUnmapNotify(e.xunmap);
                break;
            case ConfigureNotify:
                OnConfigureNotify(e.xconfigure);
                break;
            case MapRequest:
                OnMapRequest(e.xmaprequest);
                break;
            case ConfigureRequest:
                OnConfigureRequest(e.xconfigurerequest);
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
                //     this->CurrentDisplay, e.xmotion.window, MotionNotify, &e))
                // {
                // }
                // OnMotionNotify(e.xmotion);
                break;
            case KeyPress:
                OnKeyPress(e.xkey);
                break;
            case KeyRelease:
                // OnKeyRelease(e.xkey);
                break;
            default:
                LOG(WARNING) << "Ignored event";
            }
        }
    }

    static int OnXError(Display *display, XErrorEvent *e)
    {

        const int MAX_ERROR_TEXT_LENGTH = 1024;
        char error_text[MAX_ERROR_TEXT_LENGTH];
        XGetErrorText(display, e->error_code, error_text, sizeof(error_text));

        return 0;
        
    }
};