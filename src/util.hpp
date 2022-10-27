#include <X11/Xutil.h>
#include <unordered_map>
#include <glog/logging.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <X11/cursorfont.h>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#ifndef __UTIL__
#define __UTIL__

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B) ((A) <= (X) && (X) <= (B))

struct Loc
{
	int x;
	int y;
};

void start(char *what);
std::string exec(const char* cmd);
std::string ToString(const XEvent &e);

void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);

enum Layouts
{
	Layouts_Horizontal,
	Layouts_Vertical
};


enum
{
	CurNormal,
	CurResize,
	CurMove,
	CurLast
};

void Log(std::string log);

void DrawText(Display *display, Drawable drawable, int screen, std::string Color, int x, int y, char *text);

std::string GetTime();
std::string ToString(const XEvent &e);
std::string GetDate();


#endif