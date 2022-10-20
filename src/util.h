#include <X11/Xutil.h>
#include <unordered_map>
#include <glog/logging.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <fstream>

#ifndef __UTIL__
#define __UTIL__

#define TOP_BAR_HEIGHT 20
#define TAG_LENGHT 30
#define GAP 10
#define BORDER_WIDTH 3
#define HOTKEY Mod4Mask

#define TOPBAR_FG 0x707071
#define TOPBAR_BG 0x002440
#define TOPBAR_SELECTED_FG 0xE72264
#define CLIENT_NORMAL_BCOLOR 0x3B3B3B
#define CLIENT_SELECTED_BCOLOR 0x1E88E5

static const char *fonts[] = {"monospace:size=10"};

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B) ((A) <= (X) && (X) <= (B))

struct Loc
{
	int x;
	int y;
};

void start(char *what);
std::string ToString(const XEvent &e);

void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);

enum Layouts
{
	Layouts_Horizontal,
	Layouts_Vertical
};

void Log(std::string log);

#endif