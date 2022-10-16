#include "monitor.h"

Monitor::Monitor(Display *display) : _Display(display) , Tags({})
{
}

Monitor::~Monitor() {}

void Monitor::setTags(std::list<Tag *> tags)
{

    this->Tags = tags;
}

void Monitor::DrawBar()
{

    // Drawable* drw = drw_create(dpy, screen, root, sw, sh);
    // XSetForeground(drw->dpy, drw->gc, invert ? drw->scheme[ColBg].pixel : drw->scheme[ColFg].pixel);
    // XFillRectangle(drw->dpy, drw->drawable, drw->gc, x, y, w, h);
}
