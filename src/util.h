#include <X11/Xutil.h>
#include <unordered_map>
#include <glog/logging.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <fstream>
#include <chrono>
#include <ctime>   

#ifndef __UTIL__
#define __UTIL__

#define TOP_BAR_HEIGHT 33
#define TAG_LENGHT 35
#define GAP 7
#define BORDER_WIDTH 3
#define HOTKEY Mod1Mask

#define TOPBAR_FG "#A0A0A0"
#define TOPBAR_BG 0x000000
#define TOPBAR_SELECTED_FG "#ff00a6"
#define CLIENT_NORMAL_BCOLOR 0x3B3B3B
#define CLIENT_SELECTED_BCOLOR 0x1E88E5
#define TOPBAR_FONT "monospace-7"

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

void DrawText(Display *display, Drawable drawable, int screen, std::string Color, int x, int y, char *text);

std::string GetTime();
std::string GetDate();

#endif