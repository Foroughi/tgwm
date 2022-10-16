#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <list>
#include <unordered_map>
#include "tag.h"
#include "util.h"

#ifndef __Monitor__
#define __Monitor__

class Monitor
{

private:
    Loc Location;
    Loc Size;
    Loc WindowLocatin;
    Loc WindowSize;
    std::list<Tag *> Tags;
    Tag *SelectedTag;
    Display* _Display;

public:
    Monitor(Display *display);

    ~Monitor();

    void setTags(std::list<Tag *> tags);

    void DrawBar();
};

#endif