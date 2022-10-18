#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <list>
#include <vector>
#include <unordered_map>
#include "tag.h"
#include "util.h"
#include <functional>

#ifndef __Monitor__
#define __Monitor__

class Monitor
{

private:
    
    Loc Location;
    Loc Size;
    Loc WindowLocatin;
    Loc WindowSize;
    std::vector<Tag *> Tags;
    Tag *SelectedTag;
    Display* _Display;
    int Screen;

public:
    Monitor(Display *display , int screen , std::vector<Tag*> tags);

    ~Monitor();

    void setTags(std::vector<Tag *> tags);
    std::vector<Tag *> GetTags();
    Loc GetSize();
    void SetSize(int x, int y);
    Loc GetLoc();
    void SetLoc(int x , int y);
    int GetScreen();
    Tag * GetSelectedTag();
    void SelectTagByIndex(int index);    
    std::function<void(int)> OnSelectedTagChanged = NULL;
    
};

#endif