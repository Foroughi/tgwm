#include "manager.h"

#define TOP_BAR_HEIGHT 20
#define TAG_LENGHT 30
#define GAP 10
#define BORDER_WIDTH 3
#define HOTKEY Mod1Mask

void Manager::Config()
{

    std::vector<Tag *> tags[] = {
        {new Tag("sys "),
         new Tag("dev "),
         new Tag("www "),
         new Tag("term"),
         new Tag("misc")},
        {new Tag("www "),
         new Tag("misc")}};

#ifdef XINERAMA

    if (XineramaIsActive(this->CurrentDisplay))
    {

        LOG(INFO) << "using Xinerama";

        int monitorCount;

        Monitor *m;
        int w = 0;
        auto info = XineramaQueryScreens(this->CurrentDisplay, &monitorCount);

        for (int i = 0; i < monitorCount; i++)
        {
            auto mon = new Monitor(this->CurrentDisplay, info[i].screen_number,
                                   tags[info[i].screen_number]);

            LOG(INFO) << "using Xinerama" << info[i].width << " " << info[i].height;

            mon->SetSize(info[i].width, info[i].height);
            mon->SetLoc(info[i].x_org, info[i].y_org);

            mon->OnSelectedTagChanged = [=](int Index)
            {
                this->DrawBars();
            };

            // this->DrawBars();

            this->Monitors.push_back(mon);
        }
    }
    else
#endif
    {

        LOG(INFO) << "Not using Xinerama";
        auto mon = new Monitor(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay),
                               tags[0]);

        mon->SetSize(DisplayWidth(this->CurrentDisplay, mon->GetScreen()), DisplayHeight(this->CurrentDisplay, mon->GetScreen()));

        mon->OnSelectedTagChanged = [=](int Index)
        {
            this->DrawBars();
        };

        // draw = drw_create(this->CurrentDisplay, mon->GetScreen(), this->root,
        //                   mon->GetSize().x,
        //                   mon->GetSize().y);

        this->Monitors.push_back(mon);
    }
}

void Manager::onSelectedTagChanged(int Index)
{
}

Manager::Manager(Display *display) : CurrentDisplay(display), root(DefaultRootWindow(display))
{

    this->IsRunning = True;
}

Manager::~Manager()
{
    XCloseDisplay(this->CurrentDisplay);
}

void Manager::DrawBars()
{

    for (auto it : this->Monitors)
    {
        this->DrawBar(it);
    }
}

GC create_gc(Display *display, Window win, int Screen)
{
    GC gc;                       /* handle of newly created GC.  */
    unsigned long valuemask = 0; /* which values in 'values' to  */
                                 /* check when creating the GC.  */
    XGCValues values;            /* initial values for the GC.   */
    unsigned int line_width = 2; /* line width for the GC.       */
    int line_style = LineSolid;  /* style for lines drawing and  */
    int cap_style = CapButt;     /* style of the line's edje and */
    int join_style = JoinBevel;  /*  joined lines.		*/
    int screen_num = Screen;

    gc = XCreateGC(display, win, valuemask, &values);
    if (gc < 0)
    {
        fprintf(stderr, "XCreateGC: \n");
    }

    XSetForeground(display, gc, WhitePixel(display, Screen));
    XSetBackground(display, gc, BlackPixel(display, Screen));

    /* define the style of lines that will be drawn using this GC. */
    XSetLineAttributes(display, gc,
                       line_width, line_style, cap_style, join_style);

    /* define the fill style for the GC. to be 'solid filling'. */
    XSetFillStyle(display, gc, FillSolid);

    return gc;
}

void Manager::DrawBar(Monitor *mon)
{

    GC gc = create_gc(this->CurrentDisplay, this->root, mon->GetScreen());


    XSetForeground(this->CurrentDisplay, gc, 0xffffff);
    // XDrawRectangle(this->CurrentDisplay, this->root, gc, 120, 150, 50, 60);
    XFillRectangle(this->CurrentDisplay, this->root, gc, mon->GetLoc().x,  mon->GetLoc().y, mon->GetSize().x, TOP_BAR_HEIGHT);

    // int num_stuff = sizeof(stuff) / sizeof(XTextItem);

    XSetForeground(this->CurrentDisplay, gc, BlackPixel(this->CurrentDisplay, mon->GetScreen()));
    // XSetBackground(display, gc, BlackPixel(display, screen_num));

    auto i = 0;
    for (auto it : mon->GetTags())
    {
        if (it == mon->GetSelectedTag())
            XSetForeground(this->CurrentDisplay, gc, 0xff0000);
        else
            XSetForeground(this->CurrentDisplay, gc, BlackPixel(this->CurrentDisplay, mon->GetScreen()));

        XTextItem stuff[] = {
            {it->GetName().data(), it->GetName().length(), 5, None}};

        XDrawText(this->CurrentDisplay, this->root, gc, mon->GetLoc().x + (i * TAG_LENGHT), 13, stuff, 1);

        i++;
    }
}

void Manager::Unframe(Window w)
{

    // const Window frame = clients_[w];

    const Window frame = clients_[w];

    XUnmapWindow(this->CurrentDisplay, frame);
    XReparentWindow(this->CurrentDisplay, w, this->root, 0, 0);
    XRemoveFromSaveSet(this->CurrentDisplay, w);
    XDestroyWindow(this->CurrentDisplay, frame);

    clients_.erase(w);

    LOG(INFO) << "Unframed window " << w << " [" << frame << "]";
}

int updatenumlockmask(Display *dpy)
{
    unsigned int i, j;
    XModifierKeymap *modmap;

    int numlockmask = 0;
    modmap = XGetModifierMapping(dpy);
    for (i = 0; i < 8; i++)
        for (j = 0; j < modmap->max_keypermod; j++)
            if (modmap->modifiermap[i * modmap->max_keypermod + j] == XKeysymToKeycode(dpy, XK_Num_Lock))
                numlockmask = (1 << i);
    XFreeModifiermap(modmap);

    return numlockmask;
}

void grabkeys(Display *dpy, Window win)
{
    auto numlockmask = updatenumlockmask(dpy);

    unsigned int modifiers[] = {0, LockMask, numlockmask, numlockmask | LockMask};

    XUngrabKey(dpy, AnyKey, AnyModifier, win);
    for (int i = 0; i < 4; i++)
    {
        auto h = HOTKEY | modifiers[i];

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_F1), h, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_F2), h, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_F3), h, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_1), h, win,
                 True, GrabModeAsync, GrabModeAsync);
        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_2), h, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_3), h, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_4), h, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_5), h, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_6), h, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_7), h, win,
                 True, GrabModeAsync, GrabModeAsync);
    }
}

void Manager::Frame(Window w, bool was_created_before_window_manager)
{

    const unsigned long BORDER_COLOR = 0xff0000;
    const unsigned long BG_COLOR = 0x0000ff;

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

    const Window frame = XCreateSimpleWindow(this->CurrentDisplay, this->root, GAP, TOP_BAR_HEIGHT + GAP, this->Monitors[0]->GetSize().x - 20, this->Monitors[0]->GetSize().y - 20 - TOP_BAR_HEIGHT, BORDER_WIDTH, BORDER_COLOR, BG_COLOR);

    // XSelectInput(this->CurrentDisplay, frame, EnterWindowMask | FocusChangeMask | PropertyChangeMask | StructureNotifyMask);
    XSelectInput(
        this->CurrentDisplay,
        frame,
        SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask);

    XAddToSaveSet(this->CurrentDisplay, w);
    XReparentWindow(this->CurrentDisplay, w, frame, 0, 0);
    XMapWindow(this->CurrentDisplay, frame);
    clients_[w] = frame;
    XResizeWindow(this->CurrentDisplay, w, (this->Monitors[0]->GetSize().x - 20), this->Monitors[0]->GetSize().y - 20 - TOP_BAR_HEIGHT);
    this->Monitors[0]->GetSelectedTag()->AddClient(this->CurrentDisplay, frame);

    // XGrabButton(this->CurrentDisplay, Button1, Mod1Mask, w, false, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    // XGrabButton(this->CurrentDisplay, Button3, Mod1Mask, w, false, ButtonPressMask | ButtonReleaseMask | ButtonMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    // XGrabKey(this->CurrentDisplay, XKeysymToKeycode(this->CurrentDisplay, XK_F4), Mod1Mask, w, false, GrabModeAsync, GrabModeAsync);
    // XGrabKey(this->CurrentDisplay, XKeysymToKeycode(this->CurrentDisplay, XK_F1), HOTKEY, w, false, GrabModeAsync, GrabModeAsync);

    LOG(INFO) << "Framed window " << w << " [" << frame << "]";
}

void Manager::OnConfigureRequest(const XConfigureRequestEvent &e)
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

int Manager::OnXError(XErrorEvent *e)
{

    const int MAX_ERROR_TEXT_LENGTH = 1024;
    char error_text[MAX_ERROR_TEXT_LENGTH];
    XGetErrorText(this->CurrentDisplay, e->error_code, error_text, sizeof(error_text));

    return 0;
}

void Manager::OnMapRequest(const XMapRequestEvent &e)
{
    Frame(e.window, false);
    XMapWindow(this->CurrentDisplay, e.window);
}

void Manager::OnCreateNotify(const XCreateWindowEvent &e) {}

void Manager::OnDestroyNotify(const XDestroyWindowEvent &e) {}

void Manager::OnReparentNotify(const XReparentEvent &e) {}

void Manager::OnMapNotify(const XMapEvent &e) {}

void Manager::OnUnmapNotify(const XUnmapEvent &e)
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

void Manager::OnConfigureNotify(const XConfigureEvent &e) {}

void Manager::reparentAlreadyOpenWindows()
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

void Manager::OnMouseEnter(const XCrossingEvent &e){

    Client *c;
	Monitor *m;
	
	XSetInputFocus(this->CurrentDisplay, e.window, RevertToPointerRoot, CurrentTime);
}

void Manager::OnKeyPress(const XKeyEvent &e)
{
    if(e.type != MotionNotify)
        LOG(INFO) << e.type;

    if (e.state & HOTKEY)
    {
        if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("F1")))
        {
            IsRunning = False;
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("1")))
        {
            this->Monitors[0]->SelectTagByIndex(0);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("2")))
        {
            this->Monitors[0]->SelectTagByIndex(1);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("3")))
        {
            this->Monitors[0]->SelectTagByIndex(2);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("4")))
        {
            this->Monitors[0]->SelectTagByIndex(3);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("5")))
        {
            this->Monitors[0]->SelectTagByIndex(4);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("6")))
        {
            this->Monitors[1]->SelectTagByIndex(0);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("7")))
        {
            this->Monitors[1]->SelectTagByIndex(1);
        }

        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("F2")))
        {
            start("xterm");

            // reparentAlreadyOpenWindows(display , this->root);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("F3")))
        {
            start("code");

            // reparentAlreadyOpenWindows(display , this->root);
        }
    }
}

void Manager::Run()
{

    XSelectInput(
        this->CurrentDisplay,
        this->root,
        SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask | PropertyChangeMask);

    XSync(this->CurrentDisplay, false);

    grabkeys(this->CurrentDisplay, this->root);

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

    // XGrabKey(this->CurrentDisplay, XKeysymToKeycode(this->CurrentDisplay, XK_F1), HOTKEY, this->root, True, GrabModeAsync, GrabModeAsync);

    this->Config();
    this->DrawBars();

    while (IsRunning)
    {

        XEvent e;
        XNextEvent(this->CurrentDisplay, &e);
        if(e.type != MotionNotify)
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
        case EnterNotify :
            OnMouseEnter(e.xcrossing);
        default:
            LOG(WARNING) << "Ignored event";
        }
    }
}

int Manager::OnXError(Display *display, XErrorEvent *e)
{

    const int MAX_ERROR_TEXT_LENGTH = 1024;
    char error_text[MAX_ERROR_TEXT_LENGTH];
    XGetErrorText(display, e->error_code, error_text, sizeof(error_text));

    return 0;
}