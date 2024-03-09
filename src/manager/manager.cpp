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
            mon->SetWidgets(CONFIG::Widgets.at(i));
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

        mon->SetWidgets(CONFIG::Widgets.at(0));

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
}

Manager::~Manager()
{
    XCloseDisplay(this->CurrentDisplay);
}

void Manager::Reload()
{

    for (auto it : this->Monitors)
    {
        for (auto w : it->GetWidgets())
        {
            w->SetChangeStatus(true);
        }
    }

    this->UpdateWidgets();
    this->DrawBars();
    this->SortAll();
}

void Manager::DrawBars()
{

    for (auto it : this->Monitors)
    {

        this->DrawBar(it);
    }

    // this->UpdateWidgets();
}

void Manager::DrawBar(Monitor *mon)
{

    XftColor color;

    auto font = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), TOPBAR_FONT);
    auto iconfont = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), ICON_FONT);
    auto font_sub = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), TOPBAR_FONT_SUB);

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

        w = TAGGAP;

        if (it->GetIcon().length() > 0)
        {

            XGlyphInfo iconextents;
            XftTextExtentsUtf8(this->CurrentDisplay, iconfont, (FcChar8 *)it->GetIcon().data(), 3, &iconextents);

            XftDrawStringUtf8(d, it == mon->GetSelectedTag() ? &selectedcolor : &normalcolor, iconfont, x + w, 18, (const FcChar8 *)it->GetIcon().data(), 3);

            w += iconextents.width;
        }

        if (it->GetName().length() > 0)
        {

            XGlyphInfo extents;
            XftTextExtentsUtf8(this->CurrentDisplay, font, (FcChar8 *)it->GetName().data(), it->GetName().length(), &extents);

            if (it->GetHoverStatus())
            {
                XftDrawRect(d, &selectedcolor, x, 0, extents.width + (TAGGAP * 2), TOP_BAR_HEIGHT);
                XftDrawString8(d, &bgColor, font, x + w, 18, (FcChar8 *)it->GetName().c_str(), it->GetName().length());
            }
            else
            {
                XftDrawRect(d, &bgColor, x, 0, extents.width + (TAGGAP * 2), TOP_BAR_HEIGHT);
                XftDrawString8(d, it == mon->GetSelectedTag() ? &selectedcolor : &normalcolor, font, x + w, 18, (FcChar8 *)it->GetName().c_str(), it->GetName().length());
            }

            w += extents.width;
        }

        if (mon->GetClients(it->GetIndex(), FSNormal).size() > 0)
        {
            std::string clientCountStr = std::to_string(mon->GetClients(it->GetIndex(), FSNormal).size());

            XGlyphInfo subextents;
            XftTextExtentsUtf8(this->CurrentDisplay, font_sub, (FcChar8 *)clientCountStr.data(), clientCountStr.length(), &subextents);

            XftDrawStringUtf8(d, it == mon->GetSelectedTag() ? &selectedcolor : &normalcolor, font_sub, x + w + 5, 13, (FcChar8 *)clientCountStr.c_str(), clientCountStr.length());
            w += subextents.width + 3;
        }

        if (it == mon->GetSelectedTag())
            XftDrawRect(d, &selectedcolor, x, 23, w + TAGGAP, 3);

        it->SetRect(x, 0, x + w + TAGGAP, TOP_BAR_HEIGHT);

        x += w + GAP;
    }
}

void Manager::UpdateWidgets()
{
    if (this->IsUpdatingWidgets)
        return;

    std::async(std::launch::async, [&]()
               {
                   this->IsUpdatingWidgets = True;

                //    for (auto it : this->Widgets)
                //    {
                //        it->Update();
                //    }

                   this->DrawWidgets();

                   this->IsUpdatingWidgets = False; });
}

void Manager::DrawWidgets()
{
    int i = 0;
    for (auto mon : this->GetMonitors())
    {

        auto d = XftDrawCreate(this->CurrentDisplay, mon->GetTopbar(), DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)));

        auto font = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), TOPBAR_FONT);
        auto iconfont = XftFontOpenName(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay), ICON_FONT);

        XftColor bgColor;
        XftColorAllocName(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), "#000000", &bgColor);

        // XftDrawRect(d, &bgColor, mon->GetSize().x - 600, 0, mon->GetSize().x - GAP, TOP_BAR_HEIGHT);

        auto width = (2 * GAP) + 20;

        for (auto w : mon->GetWidgets())
        {
            auto value = w->Update(mon);

            XGlyphInfo extents;
            XftTextExtentsUtf8(this->CurrentDisplay, font, (FcChar8 *)value.data(), strlen(value.data()), &extents);

            if (w->GetChangeStatus())
            {

                w->SetChangeStatus(false);

                // reset the widget
                XftDrawRect(d, &bgColor, mon->GetSize().x - width - extents.width - 9, GAP - 2, extents.width + 22, TOP_BAR_HEIGHT + GAP);

                XftColor selectedcolor;
                XftColorAllocName(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), w->GetColor().c_str(), &selectedcolor);

                XftDrawStringUtf8(d, &selectedcolor, iconfont, mon->GetSize().x - width - extents.width - 7, 18, (const FcChar8 *)w->GetIcon().c_str(), 3);

                if (strlen(value.data()) > 0)
                    XftDrawStringUtf8(d, &selectedcolor, font, mon->GetSize().x - width - extents.width + 10, 18, (const FcChar8 *)value.c_str(), strlen(value.data()));

                XftDrawRect(d, &selectedcolor, mon->GetSize().x - width - extents.width - 9, 23, extents.width + 22, 3);
                // XftDrawRect(d, &selectedcolor, this->Monitors[0]->GetSize().x - width - extents.width - 9, 0, extents.width + 22, TOP_BAR_HEIGHT);

                w->SetRect(mon->GetSize().x - width - extents.width - 9, GAP, extents.width + 22, TOP_BAR_HEIGHT + GAP);

                width += extents.width + 27;

                XftColorFree(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), &selectedcolor);
            }
            else
            {
                width += extents.width + 27;
            }
        }

        XftColorFree(this->CurrentDisplay, DefaultVisual(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), DefaultColormap(this->CurrentDisplay, DefaultScreen(this->CurrentDisplay)), &bgColor);

        i++;
    }
}

void Manager::Unframe(Window w)
{

    Client *c = this->SelectedMonitor->FindByWindow(w);

    if (!c)
        return;

    const Window frame = c->GetFrame();

    XUnmapWindow(this->CurrentDisplay, frame);
    XReparentWindow(this->CurrentDisplay, w, this->root, 0, 0);
    XRemoveFromSaveSet(this->CurrentDisplay, w);
    XDestroyWindow(this->CurrentDisplay, frame);

    this->SelectedMonitor->RemoveClient(this->SelectedMonitor->FindByWindow(w));
    this->SelectedMonitor->Sort();
    this->DrawBars();
}

Display *Manager::GetDisplay()
{
    return this->CurrentDisplay;
}

void Manager::OnClientMessage(XClientMessageEvent &e)
{

    for (auto mon : this->Monitors)
    {

        Client *c = mon->FindByWindow(e.window);

        if (!c)
            continue;

        else if (e.message_type == NET_Atom[NetActiveWindow])
        {

            if (this->SelectedClient != c)
            {
                mon->SelectTagByIndex(c->GetTagIndex());
            }
        }
        else if (e.message_type == NET_Atom[NetWMState])
        {
            if (e.data.l[1] == NET_Atom[NetWMFullscreen] || e.data.l[2] == NET_Atom[NetWMFullscreen])
                this->ToggleClientFullscreen(this->GetDisplay(), mon, c, (e.data.l[0] == 1 || (e.data.l[0] == 2 && !c->GetFullscreen())));
        }
    }
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

void Manager::ToggleClientFullscreen(Display *dpy, Monitor *mon, Client *client, int fullscreen)
{
    if (fullscreen && !client->GetFullscreen())
    {

        XChangeProperty(dpy, client->GetWindow(), this->NET_Atom[NetWMState], XA_ATOM, 32, PropModeReplace, (unsigned char *)&NET_Atom[NetWMFullscreen], 1);
        client->SetFullscreen(true);

        int w = mon->GetSize().x;
        int y = (mon->GetSize().y - TOP_BAR_HEIGHT);

        client->SetSize(w - (GAP * 2), y - TOP_BAR_HEIGHT - (GAP * 2) + 32);
        client->SetLocation(GAP + mon->GetLoc().x, TOP_BAR_HEIGHT + GAP + mon->GetLoc().y);        

    }
    else if (!fullscreen && client->GetFullscreen())
    {
        XChangeProperty(dpy, client->GetWindow(), this->NET_Atom[NetWMState], XA_ATOM, 32, PropModeReplace, (unsigned char *)0, 0);

        client->SetFullscreen(false);
        mon->Sort();
    }
}

Atom Manager::GetNETAtomByClient(Window w, Atom prop)
{
    int di;
    unsigned long dl;
    unsigned char *p = NULL;
    Atom da, atom = None;

    if (XGetWindowProperty(this->CurrentDisplay, w, prop, 0, 65536, False, XA_ATOM,
                           &da, &di, &dl, &dl, &p) == Success &&
        p)
    {
        atom = *(Atom *)p;
        XFree(p);
    }
    return atom;
}

void Manager::Frame(Window w, bool was_created_before_window_manager)
{

    bool isFloating = False;

    Atom wtype = this->GetNETAtomByClient(w, this->GetNETAtom(NetWMWindowType));

    if (wtype == this->GetNETAtom(NetWMWindowTypeDialog))
    {
        // a modal is being opened. That mean we dont consider it as a client.
        // We only make it sure it will be shown in the middle of its own parent
        isFloating = true;
    }

    XWindowAttributes x_window_attrs;
    CHECK(XGetWindowAttributes(this->CurrentDisplay, w, &x_window_attrs));

    if (was_created_before_window_manager)
    {
        if (x_window_attrs.override_redirect ||
            x_window_attrs.map_state != IsViewable)
        {
            LOG(INFO) << "Framed window canceled due to overrided attribute";
            return;
        }
    }

    // const Window frame = XCreateSimpleWindow(this->CurrentDisplay, this->root, -100, 100, 100, 100, BORDER_WIDTH, 0x000000, 0x000000);
    Window frame;

    static XWindowAttributes wa;
    XGetWindowAttributes(this->CurrentDisplay, w, &wa);

    frame = XCreateSimpleWindow(this->CurrentDisplay, this->root, 0, 0, wa.width, wa.height, BORDER_WIDTH, 0x000000, 0x000000);

    XSelectInput(
        this->CurrentDisplay,
        frame,
        SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask | EnterWindowMask | LeaveWindowMask);

    XAddToSaveSet(this->CurrentDisplay, w);
    XReparentWindow(this->CurrentDisplay, w, frame, 0, 0);
    XMapWindow(this->CurrentDisplay, frame);

    this->SelectedMonitor->AddClient(this->CurrentDisplay, isFloating ? this->GetSelectedClient() : NULL, frame, w, isFloating, this->SelectedMonitor->GetSelectedTag()->GetIndex());

    LOG(INFO) << "Framed window " << w << " [" << frame << "] " << (isFloating ? "Floating" : "");

    this->Update_NET_CLIENT_LIST();

    this->DrawBars();

    this->SelectedMonitor->Sort();
}

Client *Manager::FindClientByWin(Window win)
{
    for (auto it : this->Monitors)
    {
        auto client = it->FindByWinOrFrame(win);

        if (client)
        {
            return client;
        }
    }

    return NULL;
}

void Manager::OnConfigureRequest(const XConfigureRequestEvent &e)
{

    // Client *client = this->FindClientByWin(e.window);
    // Atom wtype = this->GetNETAtomByClient(e.window, this->GetNETAtom(NetWMWindowType));

    // if (client)
    // {
    //     LOG(INFO) << "Configuration asked for client :" << e.window;
    //     return;
    // }
    // else if (wtype == this->GetNETAtom(NetWMWindowTypeDialog))
    // {

    //     LOG(INFO) << "Configuration asked for dialog :" << e.window;
    //     int x = this->GetSelectedClient()->GetLocation().x;
    //     int y = this->GetSelectedClient()->GetLocation().y;

    //     x += this->GetSelectedClient()->GetSize().x / 2 - (e.width / 2);
    //     y += this->GetSelectedClient()->GetSize().y / 2 - (e.height / 2);

    //     if (x < this->GetSelectedMonitor()->GetLoc().x)
    //         x = this->GetSelectedMonitor()->GetLoc().x;

    //     if (y < this->GetSelectedMonitor()->GetLoc().y)
    //         y = this->GetSelectedMonitor()->GetLoc().y;

    //     XMoveWindow(this->CurrentDisplay, e.window, x, y);

    //     return;
    // }
    // else
    {
        LOG(INFO) << "Configuration asked for  :" << e.window;
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

            // const Window frame = c->GetFrame();
            // XConfigureWindow(this->CurrentDisplay, frame, e.value_mask, &changes);
            //  LOG(INFO) << "Reframing " << e.window;

            changes.x = 0;
            changes.y = 0;
            changes.width = c->GetSize().x;

            // I am not crazy , some QT app should made a small change in their size , so they react to that change
            //  here i reduse the height by 1 on change it back.
            changes.height = c->GetSize().y - 1;

            XConfigureWindow(this->CurrentDisplay, e.window, e.value_mask, &changes);

            changes.height = c->GetSize().y;

            XConfigureWindow(this->CurrentDisplay, e.window, e.value_mask, &changes);

            LOG(INFO) << "Resize " << e.window << " to " << changes.width << " " << changes.height;
            LOG(INFO) << "Move " << e.window << " to " << changes.x << " " << changes.y;
        }

        XConfigureWindow(this->CurrentDisplay, e.window, e.value_mask, &changes);
        LOG(INFO) << "Resize " << e.window << " to " << e.width << e.height;
    }
}

void Manager::OnMapRequest(const XMapRequestEvent &e)
{
    if (this->FindClientByWin(e.window))
        return;

    Frame(e.window, false);
    XMapWindow(this->CurrentDisplay, e.window);
}

void Manager::OnCreateNotify(const XCreateWindowEvent &e)
{
    // LOG(INFO) << "Creating " << e.window;
}

void Manager::OnDestroyNotify(const XDestroyWindowEvent &e)
{

    LOG(INFO) << "Destroying window " << e.window;

    Client *c = this->SelectedMonitor->FindByWindow(e.window);

    if (!c)
        return;

    const Window frame = c->GetFrame();

    const Window win = c->GetWindow();

    this->SelectedMonitor->RemoveClient(c);
    delete c;

    LOG(INFO) << "Destroyed window " << win << " [" << frame << "]";

    this->SelectedMonitor->Sort();

    this->Update_NET_CLIENT_LIST();
}

void Manager::OnReparentNotify(const XReparentEvent &e)
{
}

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

            auto dialogs = it->GetClients(-1, FloatingStatus::FSFloating);

            for (auto dialog : dialogs)
            {
                if (dialog->GetParent() == c)
                {
                    c = dialog;
                    break;
                }
            }

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
    this->DrawBars();
    bool ClickGrabbed = False;

    if (TAGS_CLICKABLE && !ClickGrabbed)
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

    if (WIDGETS_CLICKABLE && !ClickGrabbed)
    {

        for (auto mon : this->Monitors)
        {
            for (auto w : mon->GetWidgets())
            {
                Rect rect = w->GetRect();

                if (e.x > rect.x && e.x < (rect.x + rect.Width) && e.y > rect.y && e.y < (rect.y + rect.Height))
                {
                    w->Click(e.button, this);
                    ClickGrabbed = True;
                    break;
                }
            }
        }
    }
}

void Manager::OnKeyPress(const XKeyEvent &e)
{

    auto numlockmask = updatenumlockmask(this->GetDisplay());

    for (auto k : CONFIG::Keys)
    {
        if (e.keycode == XKeysymToKeycode(this->CurrentDisplay, std::get<0>(k)) && CLEANMASK(std::get<1>(k), numlockmask) == CLEANMASK(e.state, numlockmask))
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
    // Log(error_text);
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

    if (TAGS_HOVERABLE)
    {
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
}

Atom Manager::GetNETAtom(int atom)
{
    return this->NET_Atom[atom];
}

Atom Manager::GetWMAtom(int atom)
{
    return this->WM_Atom[atom];
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

std::vector<Monitor *> Manager::GetMonitors()
{
    return this->Monitors;
}

Monitor *Manager::GetMonitor(int index)
{
    return this->Monitors[index];
}

Window Manager::GetRoot()
{
    return this->root;
}

void Manager::Stop()
{
    this->IsRunning = false;
}

void Manager::Update_NET_CLIENT_LIST()
{
    XDeleteProperty(this->CurrentDisplay, root, NET_Atom[NetClientList]);
    for (auto mon : this->Monitors)
        for (auto client : mon->GetClients(-1, FSAll))
        {
            auto c = client->GetWindow();

            XChangeProperty(this->CurrentDisplay, root, NET_Atom[NetClientList],
                            XA_WINDOW, 32, PropModeAppend,
                            (unsigned char *)&c, 1);
        }
}

int Manager::SendEvent(Client *c, Atom proto)
{
    int n;
    Atom *protocols;
    int exists = 0;

    Window win = c->GetWindow();

    XEvent ev;

    if (XGetWMProtocols(this->CurrentDisplay, win, &protocols, &n))
    {
        while (!exists && n--)
            exists = protocols[n] == proto;
        XFree(protocols);
    }

    if (exists)
    {

        ev.type = ClientMessage;
        ev.xclient.window = win;
        ev.xclient.message_type = this->WM_Atom[WMProtocols];
        ev.xclient.format = 32;
        ev.xclient.data.l[0] = proto;
        ev.xclient.data.l[1] = CurrentTime;
        XSendEvent(this->CurrentDisplay, win, False, NoEventMask, &ev);
    }
    return exists;
}

void Manager::EnableDubugMod()
{
    this->DebugMode = True;
#define TGWM_DEBUG 1
}

int Manager::Run()
{

    this->NET_Atom[NetActiveWindow] = XInternAtom(this->CurrentDisplay, "_NET_ACTIVE_WINDOW", False);
    this->NET_Atom[NetSupported] = XInternAtom(this->CurrentDisplay, "_NET_SUPPORTED", False);
    this->NET_Atom[NetWMName] = XInternAtom(this->CurrentDisplay, "_NET_WM_NAME", False);
    this->NET_Atom[NetWMState] = XInternAtom(this->CurrentDisplay, "_NET_WM_STATE", False);
    this->NET_Atom[NetWMCheck] = XInternAtom(this->CurrentDisplay, "_NET_SUPPORTING_WM_CHECK", False);
    this->NET_Atom[NetWMFullscreen] = XInternAtom(this->CurrentDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    this->NET_Atom[NetWMWindowType] = XInternAtom(this->CurrentDisplay, "_NET_WM_WINDOW_TYPE", False);
    this->NET_Atom[NetWMWindowTypeDialog] = XInternAtom(this->CurrentDisplay, "_NET_WM_WINDOW_TYPE_DIALOG", False);
    this->NET_Atom[NetClientList] = XInternAtom(this->CurrentDisplay, "_NET_CLIENT_LIST", False);

    this->WM_Atom[WMProtocols] = XInternAtom(this->CurrentDisplay, "WM_PROTOCOLS", False);
    this->WM_Atom[WMDelete] = XInternAtom(this->CurrentDisplay, "WM_DELETE_WINDOW", False);
    this->WM_Atom[WMState] = XInternAtom(this->CurrentDisplay, "WM_STATE", False);
    this->WM_Atom[WMTakeFocus] = XInternAtom(this->CurrentDisplay, "WM_TAKE_FOCUS", False);

    XChangeProperty(this->CurrentDisplay, this->root, this->NET_Atom[NetSupported], XA_ATOM, 32,
                    PropModeReplace, (unsigned char *)NET_Atom, NetLast);
    XDeleteProperty(this->CurrentDisplay, this->root, this->NET_Atom[NetClientList]);

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
    this->UpdateWidgets();

    LOG(INFO) << "Starts....";

    std::chrono::time_point start = std::chrono::steady_clock::now();

    while (IsRunning)
    {

        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() > 30)
        {
            start = std::chrono::steady_clock::now();
            this->UpdateWidgets();
        }

        if (XPending(this->CurrentDisplay) == 0)
            continue;

        XEvent e;
        XNextEvent(this->CurrentDisplay, &e);
        // if (e.type != MotionNotify)
        // LOG(INFO) << "Received event: " << ToString(e);

        switch (e.type)
        {
        case ClientMessage:
            OnClientMessage(e.xclient);
            break;
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
            break;
        }
    }

    XSync(this->CurrentDisplay, False);
    XCloseDisplay(this->CurrentDisplay);
    return EXIT_SUCCESS;
}
