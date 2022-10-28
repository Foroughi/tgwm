#include "../manager/manager.hpp"
#ifdef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif /* XINERAMA */
#include "../config.hpp"
#include <X11/Xatom.h>
#include <string>
#include <glog/logging.h>
#include <X11/Xutil.h>
#include <chrono>
#include <iostream>
#include <future>
#include <X11/Xft/Xft.h>
#include <X11/cursorfont.h>

void Manager::Config()
{

    BootstrapFunction(this);

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

            // XSelectInput(
            //     this->CurrentDisplay,
            //     frame,
            //     LeaveWindowMask);

            XAddToSaveSet(this->CurrentDisplay, frame);
            // XReparentWindow(this->CurrentDisplay, this->root, frame, 0, 0);
            XMapWindow(this->CurrentDisplay, frame);

            auto mon = new Monitor(this->CurrentDisplay, info[i].screen_number, frame,
                                   CONFIG::Tags[info[i].screen_number]);

            mon->SetSize(info[i].width, info[i].height);
            mon->SetLoc(info[i].x_org, info[i].y_org);

            mon->OnSelectedTagChanged = [=](int Index)
            {
                this->SortAll();

                this->DrawBars();
            };

            mon->SetLayout(CONFIG::DefaultLayouts.at(i));
            this->Monitors.push_back(mon);
        }

        this->DrawBars();
    }
    else
#endif
    {

        const Window frame = XCreateSimpleWindow(this->CurrentDisplay, this->root, 0, 0, DisplayWidth(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), TOP_BAR_HEIGHT, 0, TOPBAR_BG, TOPBAR_BG);

        // Atom atom = XInternAtom(this->CurrentDisplay, "_NET_WM_WINDOW_OPACITY", False);
        // uint opacity = 0xCCCCCCCCCCCCCCCCCCCD;
        // XChangeProperty(this->CurrentDisplay, frame, atom, XA_CARDINAL, 32,
        //                 PropModeReplace, (unsigned char *)&opacity, 1L);

        // XSelectInput(
        //     this->CurrentDisplay,
        //     frame,
        //     LeaveWindowMask);
        XMapWindow(this->CurrentDisplay, frame);
        // XAddToSaveSet(this->CurrentDisplay, frame);

        auto mon = new Monitor(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), frame,
                               CONFIG::Tags[0]);

        mon->SetSize(DisplayWidth(this->CurrentDisplay, mon->GetScreen()), DisplayHeight(this->CurrentDisplay, mon->GetScreen()));

        mon->OnSelectedTagChanged = [=](int Index)
        {
            this->DrawBars();
        };

        this->Monitors.push_back(mon);
    }

    this->SelectedMonitor = this->Monitors[0];
}

void Manager::onSelectedTagChanged(int Index)
{
}

Manager::Manager(Display *display) : CurrentDisplay(display), root(DefaultRootWindow(display))
{
    this->IsRunning = True;

    this->Widgets = CONFIG::Widgets;
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

void Manager::DrawBar(Monitor *mon)
{

    XftColor color;

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
            if (it->GetHoverStatus())
            {
                XftDrawRect(d, &selectedcolor, x, 0, (it->GetName().length() > 0 ? extents.width + TAGGAP : 15) + TAGGAP, TOP_BAR_HEIGHT);
                XftDrawString8(d, &bgColor, font, x + TAGGAP, 18, (FcChar8 *)it->GetName().c_str(), it->GetName().length());
            }
            else
            {
                XftDrawRect(d, &bgColor, x, 0, (it->GetName().length() > 0 ? extents.width + TAGGAP : 15) + TAGGAP, TOP_BAR_HEIGHT);
                XftDrawString8(d, it == mon->GetSelectedTag() ? &selectedcolor : &normalcolor, font, x + TAGGAP, 18, (FcChar8 *)it->GetName().c_str(), it->GetName().length());
            }

            w = TAGGAP * 2 + extents.width;
        }
        else
        {
            XftDrawStringUtf8(d, it == mon->GetSelectedTag() ? &selectedcolor : &normalcolor, iconfont, x, 18, (const FcChar8 *)ICON_FA_COMPUTER, 3);
            w = TAGGAP * 2 + 15;
        }

        if (it == mon->GetSelectedTag())
            XftDrawRect(d, &selectedcolor, x, 23, (it->GetName().length() > 0 ? extents.width + TAGGAP : 15) + TAGGAP, 3);

        it->SetRect(x, 0, (it->GetName().length() > 0 ? extents.width + TAGGAP : 15) + TAGGAP, TOP_BAR_HEIGHT);

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

                   for (auto it : this->Widgets)
                   {
                       it->Update();
                   }

                   this->DrawWidgets();

                   this->IsUpdatingWidgets = False; });
}

void Manager::DrawWidgets()
{

    auto d = XftDrawCreate(this->CurrentDisplay, this->Monitors[0]->GetTopbar(), DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)));

    auto font = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), TOPBAR_FONT);
    auto iconfont = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), ICON_FONT);

    XftColor bgColor;
    XftColorAllocName(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), "#000000", &bgColor);

    XftDrawRect(d, &bgColor, this->Monitors[0]->GetSize().x - 500, 0, this->Monitors[0]->GetSize().x - GAP, TOP_BAR_HEIGHT);

    auto width = (2 * GAP) + 20;

    for (auto w : this->Widgets)
    {

        XftColor selectedcolor;
        XftColorAllocName(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), w->GetColor().c_str(), &selectedcolor);

        XGlyphInfo extents;
        XftTextExtentsUtf8(this->CurrentDisplay, font, (FcChar8 *)w->GetValue().data(), strlen(w->GetValue().data()), &extents);

        XftDrawStringUtf8(d, &selectedcolor, iconfont, this->Monitors[0]->GetSize().x - width - extents.width - 7, 18, (const FcChar8 *)w->GetIcon().c_str(), 3);

        if (strlen(w->GetValue().data()) > 0)
            XftDrawStringUtf8(d, &selectedcolor, font, this->Monitors[0]->GetSize().x - width - extents.width + 10, 18, (const FcChar8 *)w->GetValue().c_str(), strlen(w->GetValue().data()));

        XftDrawRect(d, &selectedcolor, this->Monitors[0]->GetSize().x - width - extents.width - 9, 23, extents.width + 22, 3);
        //XftDrawRect(d, &selectedcolor, this->Monitors[0]->GetSize().x - width - extents.width - 9, 0, extents.width + 22, TOP_BAR_HEIGHT);

        w->SetRect(this->Monitors[0]->GetSize().x - width - extents.width - 9, 0, extents.width + 22, TOP_BAR_HEIGHT);

        width += extents.width + 27;

        XftColorFree(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), &selectedcolor);
    }

    XftColorFree(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), &bgColor);
}

void Manager::Unframe(Window w)
{

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

        for (auto k : CONFIG::Keys)
        {
            XGrabKey(dpy, XKeysymToKeycode(dpy, std::get<0>(k)), std::get<1>(k) | m, win,
                     True, GrabModeAsync, GrabModeAsync);
        }
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

    XSelectInput(
        this->CurrentDisplay,
        frame,
        SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask | EnterWindowMask | LeaveWindowMask);

    XAddToSaveSet(this->CurrentDisplay, w);
    XReparentWindow(this->CurrentDisplay, w, frame, 0, 0);
    XMapWindow(this->CurrentDisplay, frame);

    this->SelectedMonitor->AddClient(this->CurrentDisplay, frame, w, this->SelectedMonitor->GetSelectedTag()->GetIndex());

    this->SelectedMonitor->Sort();

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
        return;

    if (e.event == this->root)
        return;

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

void Manager::OnButtonPress(XButtonPressedEvent &e)
{
    bool ClickGrabbed = False;

    if (!ClickGrabbed)
    {
        for (auto mon : this->Monitors)
        {

            if (e.subwindow == mon->GetTopbar())
            {
                for (auto it : mon->GetTags())
                {

                    if (it->GetHoverStatus())
                    {
                        mon->SelectTagByIndex(it->GetIndex());
                        ClickGrabbed = True;
                        this->DrawBar(mon);
                        break;
                    }
                }
            }

            if (ClickGrabbed)
                break;
        }
    }

    if (!ClickGrabbed)
    {
        
        for (auto w : this->Widgets)
        {
            Rect rect = w->GetRect();
              
            if (e.x > rect.x && e.x < (rect.x + rect.Width) && e.y > rect.y && e.y < (rect.y + rect.Height))
            {
                LOG(INFO) << "****************************" << rect.x << rect.y;    
                w->Click(e.button);
                ClickGrabbed = True;
            }
        }
    }
}

void Manager::OnKeyPress(const XKeyEvent &e)
{

    for (auto k : CONFIG::Keys)
    {
        if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, std::get<0>(k)) && CLEANMASK(std::get<1>(k)) == CLEANMASK(e.state))
        {
            std::get<2>(k)(this, e);
            break;
        }
    }
}

int Manager::OnXError(Display *display, XErrorEvent *e)
{

    const int MAX_ERROR_TEXT_LENGTH = 1024;
    char error_text[MAX_ERROR_TEXT_LENGTH];
    XGetErrorText(display, e->error_code, error_text, sizeof(error_text));
    LOG(INFO) << error_text;
    Log(error_text);
    return 0;
}

void Manager::onFocusIn(XFocusChangeEvent &e)
{
}

Monitor *Manager::GetSelectedMonitor()
{
    return this->SelectedMonitor;
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

    for (auto mon : this->Monitors)
    {

        if (e.subwindow == mon->GetTopbar())
        {

            for (auto tag : mon->GetTags())
            {

                auto rect = tag->GetRect();

                if (e.x > rect.x && e.x < (rect.x + rect.Width) && e.y > rect.y && e.y < (rect.y + rect.Height))
                    tag->SetHoverStatus(True);
                else
                    tag->SetHoverStatus(False);
            }

            this->DrawBar(mon);
        }
        else
        {
            for (auto tag : mon->GetTags())
            {
                tag->SetHoverStatus(False);
            }
        }

        this->DrawBar(mon);
    }
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
}

Monitor *Manager::GetMonitor(int index)
{
    return this->Monitors[index];
}

void Manager::Stop()
{
    this->IsRunning = false;
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
            OnButtonPress(e.xbutton);
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
