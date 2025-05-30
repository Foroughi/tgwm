#ifndef __UTIL__
#define __UTIL__

#include <X11/Xlib.h>
#include <string>

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B) ((A) <= (X) && (X) <= (B))
//#define CLEANMASK(mask) (mask & ~(0 | LockMask) & (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask))
#define CLEANMASK(mask , numlockmask)         (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))
//#define CLEANMASK(mask) (mask & (ShiftMask | ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask))
struct Loc
{
	int x;
	int y;
};

struct Rect
{
	int x;
	int y;
	int Height;
	int Width;
};

void start(std::string);
std::string exec(const char *cmd);
std::string ToString(const XEvent &e);
static unsigned int numlockmask = 0;
void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);
template <typename Func, typename... Args>
void runInThread(Func&& func, Args&&... args);
enum
{
	NetSupported,
	NetWMName,
	NetWMState,
	NetWMCheck,
	NetWMFullscreen,
	NetActiveWindow,
	NetWMWindowType,
	NetWMWindowTypeDialog,
	NetClientList,
    NetNumberOfWorkspaces,
    NetWorkspacesNames,
    NetCurrentWorkspace,
    NetWMDesktop,
	NetLast,
};

enum
{
	WMProtocols,
	WMDelete,
	WMState,
	WMTakeFocus,
	WMLast
};

enum FloatingStatus
{
	FSAll,
	FSNormal,
	FSFloating
};

enum Layouts
{
	Layouts_Horizontal,
	Layouts_Vertical,
    Layouts_Focus
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
