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
#define HOTKEY Mod1Mask

static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	{ col_gray3, col_gray1, col_gray2 },
	{ col_gray4, col_cyan,  col_cyan  },
};

#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B)        ((A) <= (X) && (X) <= (B))

struct Loc{
    int x;
    int y;
};

void start(char *what);
std::string ToString(const XEvent &e);

void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);

enum Layouts{
	Layouts_Horizontal,
	Layouts_Vertical
};



void Log(std::string log);


#endif