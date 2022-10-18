
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vector>
#include <unordered_map>
#include "util.h"

class Client
{

private:
    Window Win;
    bool IsHidden = False;
    Display *_Display;

    Loc PreLocation;

public:
    Client(Display *display, Window window);
    ~Client();

    void Show();
    void Hide();

    bool GetVisibilityStatus();

    Loc GetSize();
    void SetSize(int x , int y);

    Loc GetLocation();
    void SetLocation(int x , int y);
};
