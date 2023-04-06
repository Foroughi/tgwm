#include "util.hpp"

#include <X11/Xutil.h>
#include <glog/logging.h>

#include <X11/Xft/Xft.h>
#include <X11/cursorfont.h>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>


void start(char *what)
{
    if (!fork())
    {
        char *const args[] = {"/bin/sh", "-c", what, NULL};
        execvp("/bin/sh", args);
        exit(1);
    }
}

std::string exec(const char *cmd)
{

    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}


void die(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    if (fmt[0] && fmt[strlen(fmt) - 1] == ':')
    {
        fputc(' ', stderr);
        perror(NULL);
    }
    else
    {
        fputc('\n', stderr);
    }

    exit(1);
}

void *ecalloc(size_t nmemb, size_t size)
{
    void *p;

    if (!(p = calloc(nmemb, size)))
        die("calloc:");
    return p;
}

void Log(std::string log)
{

    std::ofstream MyFile("tgwm-log");
    MyFile << log;
    MyFile.close();
}

void DrawText(Display *display, Drawable drawable, int screen, std::string Color, int x, int y, char *text)
{
    auto font = XftFontOpenName(display, screen, "monospace-9");

    auto d = XftDrawCreate(display, drawable, DefaultVisual(display, screen), DefaultColormap(display, screen));

    XftColor color;
    XftColorAllocName(display, DefaultVisual(display, screen), DefaultColormap(display, screen), Color.data(), &color);

    XftDrawString8(d, &color, font, x, y, (FcChar8 *)text, 4);
}

std::string GetTime()
{
    return exec("timedatectl").substr(98, 5);
}

std::string GetDate()
{
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    auto c = std::ctime(&time);

    return std::string(c).substr(0, 3) + ", " + std::string(c).substr(4, 3) + " " + std::string(c).substr(9, 2);
}


std::string ToString(const XEvent &e)
{
    static const char *const X_EVENT_TYPE_NAMES[] = {
        "",
        "",
        "KeyPress",
        "KeyRelease",
        "ButtonPress",
        "ButtonRelease",
        "MotionNotify",
        "EnterNotify",
        "LeaveNotify",
        "FocusIn",
        "FocusOut",
        "KeymapNotify",
        "Expose",
        "GraphicsExpose",
        "NoExpose",
        "VisibilityNotify",
        "CreateNotify",
        "DestroyNotify",
        "UnmapNotify",
        "MapNotify",
        "MapRequest",
        "ReparentNotify",
        "ConfigureNotify",
        "ConfigureRequest",
        "GravityNotify",
        "ResizeRequest",
        "CirculateNotify",
        "CirculateRequest",
        "PropertyNotify",
        "SelectionClear",
        "SelectionRequest",
        "SelectionNotify",
        "ColormapNotify",
        "ClientMessage",
        "MappingNotify",
        "GeneralEvent",
    };

    return X_EVENT_TYPE_NAMES[e.type];
}
