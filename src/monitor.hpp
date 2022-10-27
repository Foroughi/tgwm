

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <list>
#include <vector>
#include <unordered_map>
#include "tag.hpp"
#include "client.hpp"
#include "util.hpp"
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
    std::vector<Client *> GetClients(int tagIndex);
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
    void AddClient(Display *display, Window frame, Window win, int tagIndex);    
    void RemoveClient(Client * client);
    void Sort();
    Client* FindByWindow(Window win);
    Client *FindByFrame(Window win);
    Client *FindByWinOrFrame(Window win);
    void MoveSelectedClient(int index);
    void AddClient(Client * c);
    void SetLayout(Layouts layout);

    std::function<void(int)> OnSelectedTagChanged = NULL;
};



