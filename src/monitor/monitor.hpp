#ifndef __MONITOR__
#define __MONITOR__

#include <X11/Xlib.h>
#include "../util/util.hpp"
#include <vector>
#include "../tag/tag.hpp"
#include "../client/client.hpp"
#include <functional>

class Monitor
{

private:
    Loc Location;
    Loc Size;
    Loc WindowLocatin;
    Loc WindowSize;
    Layouts _Layout;
    std::vector<Tag *> Tags;
    std::vector<Client *> Clients;
    Tag *SelectedTag;
    Window Topbar;

    Display *_Display;
    int Screen;

public:
    Monitor(Display *display, int screen, Window topbar, std::vector<Tag *> tags);

    ~Monitor();

    void setTags(std::vector<Tag *> tags);
    std::vector<Tag *> GetTags();
    std::vector<Client *> GetClients(int tagIndex , FloatingStatus FloatingStatus);
    Loc GetSize();
    void SetSize(int x, int y);
    Loc GetLoc();
    void SetLoc(int x, int y);
    int GetScreen();
    Tag *GetSelectedTag();
    void SelectTagByIndex(int index);
    Window GetTopbar();
    void HideClients(int tagIndex);
    void ShowClients(int tagIndex);
    void AddClient(Display *display,Client* parent , Window frame, Window win,bool isFloating , int tagIndex);    
    void RemoveClient(Client * client);
    void Sort();
    Client* FindByWindow(Window win);
    Client* FindByFrame(Window win);
    Client* FindByWinOrFrame(Window win);
    void MoveSelectedClient(int index);
    void AddClient(Client * c);
    void SetLayout(Layouts layout);
    void SortDialogs(Client* parent);
    std::function<void(int)> OnSelectedTagChanged = NULL;
};



#endif