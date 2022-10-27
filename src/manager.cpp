#include "manager.hpp"

void Manager::Config()
{
  
    start("nitrogen --restore");
    start("compton");

    std::vector<Tag *> tags[] = {
        {new Tag(0, "", ICON_FA_COMPUTER),
         new Tag(1, "dev", ""),
         new Tag(2, "www", ""),
         new Tag(3, "term", ""),
         new Tag(4, "misc", "")},
        {new Tag(0, "www", ""),
         new Tag(1, "misc", "")}};

#ifdef XINERAMA

    if (XineramaIsActive(this->CurrentDisplay))
    {
        int monitorCount;

        Monitor *m;

        auto info = XineramaQueryScreens(this->CurrentDisplay, &monitorCount);

        for (int i = 0; i < monitorCount; i++)
        {

            const Window frame = XCreateSimpleWindow(this->CurrentDisplay, this->root, info[i].x_org + GAP, info[i].y_org + GAP, info[i].width - (GAP * 2), TOP_BAR_HEIGHT - (1 * GAP), 0, TOPBAR_BG, TOPBAR_BG);

            Atom atom = XInternAtom(this->CurrentDisplay, "_NET_WM_WINDOW_OPACITY", False);
            uint opacity = 0xCCCCCCCCCCCCCCCCCCCD;
            XChangeProperty(this->CurrentDisplay, frame, atom, XA_CARDINAL, 32,
                            PropModeReplace, (unsigned char *)&opacity, 1L);

            XAddToSaveSet(this->CurrentDisplay, frame);
            // XReparentWindow(this->CurrentDisplay, this->root, frame, 0, 0);
            XMapWindow(this->CurrentDisplay, frame);

            auto mon = new Monitor(this->CurrentDisplay, info[i].screen_number, frame,
                                   tags[info[i].screen_number]);

            mon->SetSize(info[i].width, info[i].height);
            mon->SetLoc(info[i].x_org, info[i].y_org);

            mon->OnSelectedTagChanged = [=](int Index)
            {
                this->SortAll();
                this->DrawBars();
            };

            mon->SetLayout(DefaultLayouts.at(i));
            this->Monitors.push_back(mon);
        }

        this->DrawBars();
    }
    else
#endif
    {

        const Window frame = XCreateSimpleWindow(this->CurrentDisplay, this->root, 0, 0, DisplayWidth(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), TOP_BAR_HEIGHT, 0, TOPBAR_BG, TOPBAR_BG);

        // XAddToSaveSet(this->CurrentDisplay, frame);
        // XReparentWindow(this->CurrentDisplay, this->root, frame, 0, 0);
        XMapWindow(this->CurrentDisplay, frame);

        auto mon = new Monitor(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), frame,
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

    this->SelectedMonitor = this->Monitors[0];
}

void Manager::onSelectedTagChanged(int Index)
{
}

std::vector<Widget *> GetWidgetsConfig()
{
    
    auto i = 0;
    return {
        new Widget(
            "time", Colors[i++], ICON_FA_CLOCK,
            [](Widget *w)
            {
                return GetTime();
            },
            [](int button) {}),
        new Widget(
            "date", Colors[i++], ICON_FA_CALENDAR,
            [](Widget *w)
            { return GetDate(); },
            [](int button) {}),
        new Widget(
            "volumn", Colors[i++], ICON_FA_VOLUME_HIGH,
            [](Widget *w)
            {
                std::string volumn = exec("amixer sget Master | grep 'Left:' | awk -F'[][]' '{ print $2 }'");

                volumn = volumn.substr(0, volumn.length() - 2);

                int volumnInt = std::stoi(volumn);

                if (volumnInt < 30)
                    w->SetIcon(ICON_FA_VOLUME_OFF);
                else if (volumnInt >= 30 && volumnInt < 60)
                    w->SetIcon(ICON_FA_VOLUME_LOW);
                if (volumnInt >= 60)
                    w->SetIcon(ICON_FA_VOLUME_HIGH);

                return "";
            },
            [](int button)
            {
                start("pavucontrol");
            }),
        new Widget(
            "network", Colors[i++], ICON_FA_WIFI,
            [](Widget *w)
            { return ""; },
            [](int button) {}),
        new Widget(
            "cpu", Colors[i++], ICON_FA_MICROCHIP,
            [](Widget *w)
            {
                return exec("cat /proc/stat |grep cpu |tail -1|awk '{print ($5*100)/($2+$3+$4+$5+$6+$7+$8+$9+$10)}'|awk '{print  100-$1}'").substr(0 ,1 ) + "%";
            },

            [](int button) {}),
        new Widget(
            "memory", Colors[i++], ICON_FA_MEMORY,
            [](Widget *w)
            {
                std::string memory = exec("free -h | grep Mem:");

                return memory.substr(16, 2) + "/" + memory.substr(27, 6);
            },
            [](int button) {})

    };
}

Manager::Manager(Display *display) : CurrentDisplay(display), root(DefaultRootWindow(display))
{
    this->IsRunning = True;

    this->Widgets = GetWidgetsConfig();
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

    this->UpdateWidgets();
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

    XftColor color;

    /* Xft. */
    auto font = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), TOPBAR_FONT);
    auto iconfont = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), ICON_FONT);

    auto d = XftDrawCreate(this->CurrentDisplay, mon->GetTopbar(), DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)));

    XftColor normalcolor;
    XftColorAllocName(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), TOPBAR_FG, &normalcolor);

    XftColor selectedcolor;
    XftColorAllocName(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), TOPBAR_SELECTED_FG, &selectedcolor);

    XftColor bgColor;
    XftColorAllocName(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), "#000000", &bgColor);

    XftDrawRect(d, &bgColor, 0, 0, 300, TOP_BAR_HEIGHT);

    int x = 10;
    int w = 0;
    for (auto it : mon->GetTags())
    {
        XGlyphInfo extents;
        XftTextExtentsUtf8(this->CurrentDisplay, font, (FcChar8 *)it->GetName().data(), it->GetName().length(), &extents);

        if (it->GetName().length() > 0)
        {
            XftDrawString8(d, it == mon->GetSelectedTag() ? &selectedcolor : &normalcolor, font, x + TAGGAP, 18, (FcChar8 *)it->GetName().c_str(), it->GetName().length());
            w = TAGGAP * 2 + extents.width;
        }
        else
        {
            XftDrawStringUtf8(d, it == mon->GetSelectedTag() ? &selectedcolor : &normalcolor, iconfont, x, 18, (const FcChar8 *)ICON_FA_COMPUTER, 3);
            w = TAGGAP * 2 + 15;
        }

        if (it == mon->GetSelectedTag())
            XftDrawRect(d, &selectedcolor, x, 23, (it->GetName().length() > 0 ? extents.width + TAGGAP : 15) + TAGGAP, 3);

        x += w;
    }
}

void Manager::UpdateWidgets()
{
    if (this->IsUpdatingWidgets)
        return;

    std::async(std::launch::async, [&]()
               { 
        
        this->IsUpdatingWidgets = True;
        
        for(auto it : this->Widgets)            
        {
            it->Update();

            //std::this_thread::sleep_for(std::chrono::seconds(3));     
        }

                   
        //std::this_thread::sleep_for(std::chrono::seconds(5));     

        this->DrawWidgets(); 

        this->IsUpdatingWidgets = False;

    });
}

void Manager::DrawWidgets()
{

    auto d = XftDrawCreate(this->CurrentDisplay, this->Monitors[0]->GetTopbar(), DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)));

    auto font = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), TOPBAR_FONT);
    auto iconfont = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), ICON_FONT);

    XftColor bgColor;
    XftColorAllocName(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), "#000000", &bgColor);

    XftDrawRect(d, &bgColor, this->Monitors[0]->GetSize().x - 500, 0,  this->Monitors[0]->GetSize().x - GAP , TOP_BAR_HEIGHT);

    auto width = (2 * GAP) + 20;

    for (auto w : this->Widgets)
    {

        XftColor selectedcolor;
        XftColorAllocName(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), w->GetColor().c_str(), &selectedcolor);

        XGlyphInfo extents;
        XftTextExtentsUtf8(this->CurrentDisplay, font, (FcChar8 *)w->GetValue().data(), strlen(w->GetValue().data()), &extents);

        //XftDrawRect(d, &bgColor, this->Monitors[0]->GetSize().x - width - extents.width - 9, 0, extents.width + 22, TOP_BAR_HEIGHT);

        XftDrawStringUtf8(d, &selectedcolor, iconfont, this->Monitors[0]->GetSize().x - width - extents.width - 7, 18, (const FcChar8 *)w->GetIcon().c_str(), 3);

        if (strlen(w->GetValue().data()) > 0)
            XftDrawStringUtf8(d, &selectedcolor, font, this->Monitors[0]->GetSize().x - width - extents.width + 10, 18, (const FcChar8 *)w->GetValue().c_str(), strlen(w->GetValue().data()));

        XftDrawRect(d, &selectedcolor, this->Monitors[0]->GetSize().x - width - extents.width - 9, 23, extents.width + 22, 3);

        width += extents.width + 27;

        XftColorFree(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), &selectedcolor);
    }

    XftColorFree(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), &bgColor);


    
}

void Manager::Unframe(Window w)
{

    // const Window frame = clients_[w];
    Client *c = this->SelectedMonitor->FindByWindow(w);

    const Window frame = c->GetFrame();

    XUnmapWindow(this->CurrentDisplay, frame);
    XReparentWindow(this->CurrentDisplay, w, this->root, 0, 0);
    XRemoveFromSaveSet(this->CurrentDisplay, w);
    XDestroyWindow(this->CurrentDisplay, frame);

    this->SelectedMonitor->RemoveClient(this->SelectedMonitor->FindByWindow(w));

    this->SelectedMonitor->Sort();

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
        auto m = modifiers[i];

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_F4), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_F4), HOTKEY | m | ControlMask, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_1), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_1), HOTKEY | m | ControlMask, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_2), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_2), HOTKEY | m | ControlMask, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_3), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_3), HOTKEY | m | ControlMask, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_4), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_4), HOTKEY | m | ControlMask, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_5), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_5), HOTKEY | m | ControlMask, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_6), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_6), HOTKEY | m | ControlMask, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_7), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_7), HOTKEY | m | ControlMask, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_x), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_grave), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_Pause), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_Print), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_Print), HOTKEY | m, win,
                 True, GrabModeAsync, GrabModeAsync);

        XGrabKey(dpy, XKeysymToKeycode(dpy, XK_Print), HOTKEY | m | ControlMask, win,
                 True, GrabModeAsync, GrabModeAsync);
    }
}

void Manager::Frame(Window w, bool was_created_before_window_manager)
{

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

    const Window frame = XCreateSimpleWindow(this->CurrentDisplay, this->root, -100, 100, 100, 100, BORDER_WIDTH, 0x000000, 0x000000);

    // XSelectInput(this->CurrentDisplay, frame, EnterWindowMask | FocusChangeMask | PropertyChangeMask | StructureNotifyMask);
    XSelectInput(
        this->CurrentDisplay,
        frame,
        SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask | EnterWindowMask | LeaveWindowMask | EnterWindowMask);

    XAddToSaveSet(this->CurrentDisplay, w);
    XReparentWindow(this->CurrentDisplay, w, frame, 0, 0);
    XMapWindow(this->CurrentDisplay, frame);
    // clients_[w] = frame;

    this->SelectedMonitor->AddClient(this->CurrentDisplay, frame, w, this->SelectedMonitor->GetSelectedTag()->GetIndex());

    this->SelectedMonitor->Sort();
    // XResizeWindow(this->CurrentDisplay, w, (this->SelectedMonitor->GetSize().x - 20), this->SelectedMonitor->GetSize().y - 20 - TOP_BAR_HEIGHT);

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

    auto c = this->SelectedMonitor->FindByWindow(e.window);

    if (c != NULL)
    {

        const Window frame = c->GetFrame();
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

    if (this->SelectedMonitor->FindByWindow(e.window) == NULL)
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

void Manager::OnMouseEnter(const XCrossingEvent &e)
{

    if (e.window == this->root)
    {
        this->SelectClient(NULL);

        return;
    }

    for (auto it : this->Monitors)
    {

        auto c = it->FindByFrame(e.window);

        if (c != NULL)
        {

            if (it != this->SelectedMonitor)
                this->SelectedMonitor = it;

            this->SelectClient(c);

            break;
        }
    }
}

void Manager::OnMouseLeave(const XCrossingEvent &e)
{
}

void Manager::OnKeyPress(const XKeyEvent &e)
{
    if (e.type != MotionNotify)
        LOG(INFO) << e.type;

    if (e.state & HOTKEY)
    {
        if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("1")))
        {
            if (e.state & ControlMask)
            {
                this->MoveSelectedClient(this->Monitors[0], 0);
                this->SortAll();
            }
            else
                this->Monitors[0]->SelectTagByIndex(0);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("2")))
        {
            if (e.state & ControlMask)
            {
                this->MoveSelectedClient(this->Monitors[0], 1);
                this->SortAll();
            }
            else
                this->Monitors[0]->SelectTagByIndex(1);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("3")))
        {
            if (e.state & ControlMask)
            {
                this->MoveSelectedClient(this->Monitors[0], 2);
                this->SortAll();
            }
            else
                this->Monitors[0]->SelectTagByIndex(2);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("4")))
        {
            if (e.state & ControlMask)
            {
                this->MoveSelectedClient(this->Monitors[0], 3);
                this->SortAll();
            }
            else
                this->Monitors[0]->SelectTagByIndex(3);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("5")))
        {
            if (e.state & ControlMask)
            {
                this->MoveSelectedClient(this->Monitors[0], 4);
                this->SortAll();
            }
            else
                this->Monitors[0]->SelectTagByIndex(4);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("6")))
        {
            if (e.state & ControlMask)
            {
                this->MoveSelectedClient(this->Monitors[1], 0);
                this->SortAll();
            }
            else
                this->Monitors[1]->SelectTagByIndex(0);
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("7")))
        {
            if (e.state & ControlMask)
            {
                this->MoveSelectedClient(this->Monitors[1], 1);
                this->SortAll();
            }
            else
                this->Monitors[1]->SelectTagByIndex(1);
        }

        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("F4")))
        {

            if (e.state & ControlMask)
            {
                this->IsRunning = false;
            }
            else if (this->GetSelectedClient())
            {
                this->SelectedMonitor->RemoveClient(this->GetSelectedClient());
                this->SelectedMonitor->Sort();
            }
            // std::string str = "";

            // for (auto mon : this->Monitors)
            // {
            //     str += "Monitor (" + std::to_string(mon->GetScreen()) + "): (size :" + std::to_string(mon->GetSize().x) + ":" + std::to_string(mon->GetSize().y) + ") (loc :" + std::to_string(mon->GetLoc().x) + ":" + std::to_string(mon->GetLoc().y) + ")\n";

            //     str += "Tags :\n";
            //     for (auto it : mon->GetTags())
            //     {
            //         str += it->GetName() + "(" + std::to_string(it->GetIndex()) + ") (clients : " + std::to_string(mon->GetClients(it->GetIndex()).size()) + ") \n";
            //     }

            //     str += "\nClients :\n";
            //     for (auto it : mon->GetClients(-1))
            //     {
            //         str += "tag : (" + std::to_string(it->GetTagIndex()) + "): (size :" + std::to_string(it->GetSize().x) + ":" + std::to_string(it->GetSize().y) + ") (loc :" + std::to_string(it->GetLocation().x) + ":" + std::to_string(it->GetLocation().y) + ")\n";
            //     }
            // }

            // Log(str);
        }

        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("grave")))
        {
            start("rofi -no-lazy-grab -show drun -modi drun -config ~/.config/rofi/config.rasi");
        }
        else if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("Pause")))
        {
            start("rofi -show power-menu -modi power-menu:\"~/.config/rofi/rofi-power-menu\" -config ~/.config/rofi/config.rasi");
        }
    }
    else
    {
        if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, XStringToKeysym("Print")))
        {
            if (e.state & ControlMask)
                start("scrot -m -e 'mv $f /home/ali/Pictures/'");
            else
                start("scrot -mscrot -u -e 'mv $f /home/ali/Pictures/'");
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

void Manager::onFocusIn(XFocusChangeEvent &e)
{
}

void Manager::SortAll()
{

    for (auto it : this->Monitors)
        it->Sort();
}

void Manager::OnMotionNotify(XMotionEvent &e)
{
    if (this->Monitors.size() > 1)
    {
        if (e.x > 1920)
            this->SelectedMonitor = this->Monitors[1];
        else
            this->SelectedMonitor = this->Monitors[0];
    }
    else
        this->SelectedMonitor = this->Monitors[0];
}

Client *Manager::GetSelectedClient()
{
    return this->SelectedClient;
}

void Manager::SelectClient(Client *client)
{
    if (this->SelectedClient)

        XSetWindowBorder(this->CurrentDisplay, this->SelectedClient->GetFrame(), CLIENT_NORMAL_BCOLOR);

    if (client)
    {
        this->SelectedClient = client;
        XSetWindowBorder(this->CurrentDisplay, this->SelectedClient->GetFrame(), CLIENT_SELECTED_BCOLOR);

        XSetInputFocus(this->CurrentDisplay, client->GetWindow(), RevertToPointerRoot, CurrentTime);
    }
    else
    {
        this->SelectedClient = NULL;
        XSetInputFocus(this->CurrentDisplay, this->root, RevertToPointerRoot, CurrentTime);
    }
}

void Manager::MoveSelectedClient(Monitor *mon, int index)
{

    if (!this->SelectedClient)
        return;

    for (auto it : this->Monitors)
    {
        it->RemoveClient(this->SelectedClient);
        if (it == mon)
            it->AddClient(this->SelectedClient);
    }

    this->SelectedClient->SetTagIndex(index);
    // this->SelectedClient->Hide();
}

int Manager::Run()
{

    XSetWindowAttributes wa;

    wa.cursor = XCreateFontCursor(this->CurrentDisplay, XC_left_ptr);
    wa.event_mask = SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask | PropertyChangeMask;
    XChangeWindowAttributes(this->CurrentDisplay, this->root, CWEventMask | CWCursor, &wa);

    XSelectInput(
        this->CurrentDisplay,
        this->root,
        wa.event_mask);

    XSync(this->CurrentDisplay, false);

    grabkeys(this->CurrentDisplay, this->root);

    XSetErrorHandler(OnXError);

    this->Config();
    this->DrawBars();

    while (IsRunning)
    {

        XEvent e;
        XNextEvent(this->CurrentDisplay, &e);
        if (e.type != MotionNotify)
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
            while (XCheckTypedWindowEvent(
                this->CurrentDisplay, e.xmotion.window, MotionNotify, &e))
            {
            }
            OnMotionNotify(e.xmotion);
            break;
        case KeyPress:
            OnKeyPress(e.xkey);
            break;
        case KeyRelease:
            // OnKeyRelease(e.xkey);
            break;
        case EnterNotify:
            OnMouseEnter(e.xcrossing);
            break;
        case LeaveNotify:
            OnMouseLeave(e.xcrossing);
            break;
        case FocusIn:
        case FocusOut:
            onFocusIn(e.xfocus);
            break;
        default:
            LOG(WARNING) << "Ignored event";
        }
    }

    XSync(this->CurrentDisplay, False);
    XCloseDisplay(this->CurrentDisplay);
    return EXIT_SUCCESS;
}
