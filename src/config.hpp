

#ifndef __CONFIG__
#define __CONFIG__

#include <X11/Xutil.h>
#include <vector>
#include <string>
#include <string>
#include <algorithm>
#include "tuple"
#include "./tag/tag.hpp"
#include "./widget/widget.hpp"
#include "./client/client.hpp"
#include "./monitor/monitor.hpp"
#include "./manager/manager.hpp"
#include "./util/util.hpp"
#include <cctype>
#include <stdlib.h>

/*=====================================================================================================*/
/*=============================================== Configs =============================================*/
/*=====================================================================================================*/
#define TOP_BAR_HEIGHT 33
#define GAP 7
#define TAGGAP 5
#define BORDER_WIDTH 2
#ifdef TGWMDEBUG
/* This Hotkey will be used if the app is started with Xephyr*/
#define HOTKEY Mod1Mask
#else
/* In normal situation , when you start the app with startx or a linux DM , the TGWMDEBUG does not have a value and this hotkey will be used*/
#define HOTKEY Mod4Mask
#endif
#define TAGS_HOVERABLE 0
#define TAGS_CLICKABLE 0
#define WIDGETS_HOVERABLE 0
#define WIDGETS_CLICKABLE 1
#define TOPBAR_FG "#A0A0A0"
#define TOPBAR_BG 0x000000
#define TOPBAR_SELECTED_FG "#61afef"
#define CLIENT_NORMAL_BCOLOR 0x3B3B3B
#define CLIENT_SELECTED_BCOLOR 0x1E88E5

#define TERMINAL "kitty"
#define RUN_DBUS true
#define RUN_GNOMEKEYRING true
#define RUN_NITORGEN true
#define RUN_COMPOSITOR true
#define COMPOSITOR "picom -b"
#define RUN_CONKEY true

#define TOPBAR 0 //0- Polybar 1 - Built-in bar

/*=====================================================================================================*/
/*=============================================== Bootstrap =============================================*/
/*=====================================================================================================*/

inline std::function<void(Manager *)> BootstrapFunction = [](Manager *Manager)
{       
    if(RUN_DBUS)
        start("dbus-update-activation-environment --all");

    if(RUN_GNOMEKEYRING)
        start("gnome-keyring-daemon --start --components=secrets");

    if(RUN_NITORGEN)
        start("nitrogen --restore");
    
    if(RUN_COMPOSITOR)
        start(COMPOSITOR);
    
    if(RUN_CONKEY)
        start("conky");
                    
    if(TOPBAR == 0){
	if(Manager->GetMonitors().size() == 1)
		start("polybar");  
	else
	{
		start("polybar tgwm1");  
		start("polybar tgwm2");  
	
	}
    }
};

/*=====================================================================================================*/
/*================================================ Fonts ==============================================*/
/*=====================================================================================================*/

#define TOPBAR_FONT "hack-11:style=Bold"
#define TOPBAR_FONT_SUB "hack-8:style=Bold"
#define ICON_FONT "Font Awesome 6 Free:size=11:style=Solid"

/*=====================================================================================================*/
/*================================================ Icons ==============================================*/
/*=====================================================================================================*/
#define ICON_FA_WIFI "\xef\x87\xab"
#define ICON_FA_MICROCHIP "\xef\x8b\x9b"
#define ICON_FA_MEMORY "\xef\x94\xb8"
#define ICON_FA_KEYBOARD "\xef\x84\x9c"
#define ICON_FA_COMPUTER "\xee\x93\xa5"
#define ICON_FA_CALENDAR "\xef\x84\xb3"
#define ICON_FA_CLOCK "\xef\x80\x97"
#define ICON_FA_VOLLEYBALL "\xef\x91\x9f"
#define ICON_FA_VOLUME_HIGH "\xef\x80\xa8"
#define ICON_FA_VOLUME_LOW "\xef\x80\xa7"
#define ICON_FA_VOLUME_OFF "\xef\x80\xa6"
#define ICON_FA_VOLUME_XMARK "\xef\x9a\xa9"
#define ICON_FA_POWER_OFF "\xef\x80\x91"
#define ICON_FA_ARROWS_UP_DOWN "\xef\x81\xbd"
#define ICON_FA_ARROWS_LEFT_RIGHT "\xef\x81\xbe"
#define ICON_FA_WINDOW_MAXIMIZE "\xef\x8b\x90"

namespace CONFIG
{
    
    /*=====================================================================================================*/
    /*============================================ Default Layouts ========================================*/
    /*=====================================================================================================*/
    inline std::vector<Layouts> DefaultLayouts =
        {

            Layouts_Focus,
            Layouts_Horizontal,

    };

    /*=====================================================================================================*/
    /*============================================ Top bar ================================================*/
    /*=====================================================================================================*/
    enum Topbars
    {

            Topbar_Integrated,
            Topbar_Polybar,

    };

    /*=====================================================================================================*/
    /*============================================ Widget Colors ==========================================*/
    /*=====================================================================================================*/
    inline std::vector<std::string>
        Colors =
            {

                "#D04232",
                "#61afef",
                "#FFC12F",
                "#c678dd",
                "#B8BB26",
                "#d19a66",
                "#D3869B",
                "#98c379",
                "#c8c874",
                "#83A598",
                "#3d4059",
                "#e0e0e0",

    };

    /*=====================================================================================================*/
    /*================================================ TAGS ===============================================*/
    /*=====================================================================================================*/
    inline std::vector<Tag *> Tags[] = {
        {
        //new Tag(0, "home", ICON_FA_COMPUTER),
         new Tag(0, "term", ""),
         new Tag(1, "dev", ""),
         new Tag(2, "www", ""),         
         new Tag(3, "misc", "")},

        {new Tag(0, "www", ""),
         new Tag(1, "misc", "")}};

    /*=====================================================================================================*/
    /*================================================ Widgets ============================================*/
    /*=====================================================================================================*/

    inline std::vector<Widget *> Mon1Widgets = {

        // System Widget
        new Widget(
            "system", Colors[11], ICON_FA_POWER_OFF, 
            [](Widget *w, Monitor *mon)
            {
                return "";
            },
            [](int button, Manager *manager , Widget * widget)
            {
                start("rofi -show power-menu -modi power-menu:\"~/.config/rofi/rofi-power-menu\" -config ~/.config/rofi/config.rasi");
            }),

        // Time Widget
        new Widget(
            "time", Colors[0], ICON_FA_CLOCK, 
            [](Widget *w, Monitor *mon)
            {
                auto time = GetTime();

                if(time != w->GetValue())
                {
                    w->SetValue(time);                    
                }

                return time;
            },
            [](int button, Manager *manager , Widget * widget) {}),

        // Date Widget
        new Widget(
            "date", Colors[1], ICON_FA_CALENDAR, 
            [](Widget *w, Monitor *mon)
            { 
                auto date = GetDate(); 

                if(date != w->GetValue())
                {
                    w->SetValue(date);                    
                }

                return date;
            },
            [](int button, Manager *manager , Widget * widget) {}),

        // Volume Widget
        new Widget(
            "volumn", Colors[2], ICON_FA_VOLUME_HIGH,
            [](Widget *w, Monitor *mon)
            {
                std::string volumn = exec("amixer sget Master | grep 'Mono:' | awk -F'[][]' '{ print $2 }'");

                volumn = volumn.substr(0, volumn.length() - 2);

                int volumnInt = std::stoi(volumn);

                if (volumnInt < 30)                    
                    w->SetIcon(ICON_FA_VOLUME_OFF);
                else if (volumnInt >= 30 && volumnInt < 60)
                    w->SetIcon(ICON_FA_VOLUME_LOW);
                if (volumnInt >= 60)
                    w->SetIcon(ICON_FA_VOLUME_HIGH);

                return "";
            },
            [](int button, Manager *manager , Widget * widget)
            {
                start("pavucontrol");
            }),

        // Network Widget
        new Widget(
            "network", Colors[3], ICON_FA_WIFI,
            [](Widget *w, Monitor *mon)
            { return ""; },
            [](int button, Manager *manager , Widget * widget)
            {
                start("nm-connection-editor");
            }),

        // Cpu Widget
        new Widget(
            "cpu", Colors[4], ICON_FA_MICROCHIP, 
            [](Widget *w, Monitor *mon)
            {
                auto cpu = exec("cat /proc/stat |grep cpu |tail -1|awk '{print ($5*100)/($2+$3+$4+$5+$6+$7+$8+$9+$10)}'|awk '{print  100-$1}'").substr(0, 1) + "%";
                w->SetValue(cpu);
                return cpu;
            },
            [](int button, Manager *manager , Widget * widget)
            {
                start(std::string(TERMINAL).append(" htop"));
            }),

        // Memory Widget
        new Widget(
            "memory", Colors[5], ICON_FA_MEMORY, 
            [](Widget *w, Monitor *mon)
            {
                std::string memory = exec("free -h | grep Mem:");

                memory = memory.substr(16, 2) + "/" + memory.substr(27, 6);

                w->SetValue(memory);

                return memory;
            },
            [](int button, Manager *manager , Widget * widget) {}),

        // Memory Widget
        new Widget(
            "keyboard", Colors[6], ICON_FA_KEYBOARD,
            [](Widget *w, Monitor *mon)
            {
                std::string layout = exec("setxkbmap -query | grep layout").substr(12, 2);
                for (auto &c : layout)
                    c = toupper(c);

                w->SetValue(layout);
                return layout;
            },
            [](int button, Manager *manager , Widget * widget)
            {
                std::string layout = exec("setxkbmap -query | grep layout").substr(12, 2);

                if (layout == "de")
                    start("setxkbmap us");
                else if (layout == "us")
                    start("setxkbmap ir");
                else
                    start("setxkbmap de");
                
                widget->SetChangeStatus(true);
                manager->UpdateWidgets();
            }),

        // Layout Mon 1
        new Widget(
            "Layout", Colors[7], ICON_FA_KEYBOARD,
            [](Widget *w, Monitor *mon)
            {
                auto layout = mon->GetLayout();
                std::string layoutStr = "";

                if (layout == Layouts::Layouts_Horizontal)
                {
                    layoutStr = "Hor";
                    w->SetIcon(ICON_FA_ARROWS_UP_DOWN);
                }
                else if (layout == Layouts::Layouts_Vertical)
                {
                    layoutStr = "Ver";
                    w->SetIcon(ICON_FA_ARROWS_LEFT_RIGHT);
                }
                else
                {
                    layoutStr = "Foc";
                    w->SetIcon(ICON_FA_WINDOW_MAXIMIZE);
                }

                w->SetValue(layoutStr);
                return layoutStr;
                
            },
            [](int button, Manager *manager , Widget * widget)
            {
                auto layout = manager->GetMonitor(0)->GetLayout();

                if (layout == Layouts::Layouts_Horizontal)
                    manager->GetMonitor(0)->SetLayout(Layouts::Layouts_Vertical);

                else if (layout == Layouts::Layouts_Vertical)
                    manager->GetMonitor(0)->SetLayout(Layouts::Layouts_Focus);
                else
                    manager->GetMonitor(0)->SetLayout(Layouts::Layouts_Horizontal);

                widget->SetChangeStatus(true);
                manager->UpdateWidgets();
                manager->SortAll();
            }),        
    };

    inline std::vector<Widget *> Mon2Widgets = {

        // Time Widget
        new Widget(
            "time", Colors[0], ICON_FA_CLOCK,
            [](Widget *w, Monitor *mon)
            {
                auto time = GetTime();

                w->SetValue(time);

                return time;
            },
            [](int button, Manager *manager , Widget * widget) {}),

        // Date Widget
        new Widget(
            "date", Colors[1], ICON_FA_CALENDAR,
            [](Widget *w, Monitor *mon)
            { 
                auto date = GetDate();

                w->SetValue(date);

                return date; 
            },
            [](int button, Manager *manager , Widget * widget) {}),


        // Layout Mon 2
        new Widget(
            "Layout", Colors[7], ICON_FA_KEYBOARD, 
            [](Widget *w, Monitor *mon)
            {
                auto layout = mon->GetLayout();
                std::string layoutStr = "";

                if (layout == Layouts::Layouts_Horizontal)
                {
                    layoutStr = "Hor";
                    w->SetIcon(ICON_FA_ARROWS_UP_DOWN);
                }
                else if (layout == Layouts::Layouts_Vertical)
                {
                    layoutStr = "Ver";
                    w->SetIcon(ICON_FA_ARROWS_LEFT_RIGHT);
                }
                else
                {
                    layoutStr = "Foc";
                    w->SetIcon(ICON_FA_WINDOW_MAXIMIZE);
                }

                w->SetValue(layoutStr);
                return layoutStr;
            },
            [](int button, Manager *manager , Widget * widget)
            {
                auto layout = manager->GetMonitor(1)->GetLayout();

                if (layout == Layouts::Layouts_Horizontal)
                    manager->GetMonitor(1)->SetLayout(Layouts::Layouts_Vertical);

                else if (layout == Layouts::Layouts_Vertical)
                    manager->GetMonitor(1)->SetLayout(Layouts::Layouts_Focus);
                else
                    manager->GetMonitor(1)->SetLayout(Layouts::Layouts_Horizontal);
                
                widget->SetChangeStatus(true);
                manager->UpdateWidgets();
                manager->SortAll();
            }),
    };

    inline std::vector<std::vector<Widget *>> Widgets = {
        Mon1Widgets , Mon2Widgets
    };

    /*=====================================================================================================*/
    /*=============================================== Bindings ============================================*/
    /*=====================================================================================================*/
    inline std::vector<std::tuple<int, int, std::function<void(Manager *, const XKeyEvent)>>> Keys = {
        
        // Mod + F1
        {XK_F1, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            //  LOG(INFO) << "======================================================";

            //  for (auto mon : manager->GetMonitors())
            //  {
            //      LOG(INFO) << "Monitor (" + std::to_string(mon->GetScreen()) + "): (size :" + std::to_string(mon->GetSize().x) + ":" + std::to_string(mon->GetSize().y) + ") (loc :" + std::to_string(mon->GetLoc().x) + ":" + std::to_string(mon->GetLoc().y) + ")\n";

            //      LOG(INFO) << "Tags :\n";
            //      for (auto it : mon->GetTags())
            //      {
            //          LOG(INFO) << it->GetName() + "(" + std::to_string(it->GetIndex()) + ") (clients : " + std::to_string(mon->GetClients(it->GetIndex(), FSAll).size()) + ") \n";
            //      }

            //      LOG(INFO) << "Clients :\n";
            //      for (auto it : mon->GetClients(-1, FSAll))
            //      {
            //          LOG(INFO) << "tag : (" + std::to_string(it->GetTagIndex()) + "): (Frame size :" + std::to_string(it->GetSize().x) + ":" + std::to_string(it->GetSize().y) + ") (Frame loc :" + std::to_string(it->GetLocation().x) + ":" + std::to_string(it->GetLocation().y) + ") (Win size : "+ std::to_string(it->GetWinSize().x) + ":" + std::to_string(it->GetWinSize().y) +") (Win loc : "+ std::to_string(it->GetWinLocation().x) + ":" + std::to_string(it->GetWinLocation().y) +")\n";
            //      }
            //  }

            //  LOG(INFO) << "======================================================";
             // Log(str);
         }},

        // Mod + Ctrl + Esc
        {XK_Escape, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
             manager->Stop();
         }},

        // Mod + Esc
        {XK_Escape, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             if (manager->GetSelectedClient() && manager->GetSelectedClient()->GetWindow() != manager->GetRoot())
                 manager->SendEvent(manager->GetSelectedClient(), manager->GetWMAtom(WMDelete));
         }},

        // Mod + Ctrl + 1
        {XK_1, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
             manager->MoveSelectedClient(manager->GetMonitor(0), 0);
             manager->SortAll();
             manager->DrawBars();
         }},
        // Mod + 1
        {XK_1, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             manager->GetMonitor(0)->SelectTagByIndex(0);
         }},

        // Mod + Ctrl + 2
        {XK_2, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
             manager->MoveSelectedClient(manager->GetMonitor(0), 1);
             manager->SortAll();
             manager->DrawBars();
         }},

        // Mod + 2
        {XK_2, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             manager->GetMonitor(0)->SelectTagByIndex(1);
         }},

        // Mod + Ctrl + 3
        {XK_3, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
             manager->MoveSelectedClient(manager->GetMonitor(0), 2);
             manager->SortAll();
             manager->DrawBars();
         }},

        // Mod + 3
        {XK_3, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             manager->GetMonitor(0)->SelectTagByIndex(2);
         }},

        // Mod + Ctrl + 4
        {XK_4, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
             manager->MoveSelectedClient(manager->GetMonitor(0), 3);
             manager->SortAll();
             manager->DrawBars();
         }},

        // Mod + 4
        {XK_4, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             manager->GetMonitor(0)->SelectTagByIndex(3);
         }},

        // Mod + Ctrl + 5
        {XK_5, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
		if(manager->GetMonitors().size() > 1)
	             manager->MoveSelectedClient(manager->GetMonitor(1), 0);
             manager->SortAll();
             manager->DrawBars();
         }},

        // Mod + 5
        {XK_5, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            if(manager->GetMonitors().size() > 1)
             manager->GetMonitor(1)->SelectTagByIndex(0);
         }},

        // Mod + Ctrl + 6
        {XK_6, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
            if(manager->GetMonitors().size() > 1)
                manager->MoveSelectedClient(manager->GetMonitor(1), 1);

             manager->SortAll();
             manager->DrawBars();
         }},

        // Mod + 6
        {XK_6, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            if(manager->GetMonitors().size() > 1)
                manager->GetMonitor(1)->SelectTagByIndex(1);
         }},

        // Mod + Ctrl + 7
        {XK_7, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
            if(manager->GetMonitors().size() > 1)
                manager->MoveSelectedClient(manager->GetMonitor(1), 2);
             manager->SortAll();
             manager->DrawBars();
         }},

        // Mod + 7
        {XK_7, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            if(manager->GetMonitors().size() > 1)
                manager->GetMonitor(1)->SelectTagByIndex(2);
         }},

        // Mod + ~
        {XK_grave, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            start("rofi -no-lazy-grab -show drun -config ~/.config/rofi/config.rasi");
            //start("dmenu_run");
         }},

        // Mod + Pause
        {XK_Pause, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             start("rofi -show power-menu -modi power-menu:\"~/.config/rofi/rofi-power-menu\" -config ~/.config/rofi/config.rasi");
         }},

        // Mod + Tab
        {XK_Tab, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             start("rofi -no-lazy-grab -show window -config ~/.config/rofi/config.rasi");
         }},

        // Mod + Ctrl + Print
        {XK_Print, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
             start("scrot -m -e 'mv $f /home/ali/pictures/'");
         }},

        // Mod + Print
        {XK_Print, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             start("scrot -s  -e 'xclip -selection clipboard -t image/png -i $f && rm $f'");
         }},

        // Mod + Enter
        {XK_Return, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             start(std::string(TERMINAL).append(" bash -c 'tmux attach-session ||  tmux'"));
         }},

        // Mod + Ctrl  + Enter
        {XK_Return, HOTKEY | ControlMask , [](Manager *manager, const XKeyEvent &e)
         {
             start(TERMINAL);
         }},

        // Mod + F5
        {XK_F5, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             manager->Reload();
         }},

        // Mod + F12
        {XK_F12, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            auto layout = manager->GetSelectedMonitor()->GetLayout();

            if(layout == Layouts_Horizontal)
                manager->GetSelectedMonitor()->SetLayout(Layouts_Vertical);
            else if(layout == Layouts_Vertical)
                 manager->GetSelectedMonitor()->SetLayout(Layouts_Focus);
            else 
                manager->GetSelectedMonitor()->SetLayout(Layouts_Horizontal);                    
            manager->GetSelectedMonitor()->Sort();

            manager->GetSelectedMonitor()->UpdateWidget("Layout");
            manager->UpdateWidgets();
         }},

        // Mod + Numeric 1
        {XK_KP_1, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            auto clients = manager->GetSelectedMonitor()->GetClients(manager->GetSelectedMonitor()->GetSelectedTag()->GetIndex() , FloatingStatus::FSNormal);
            
            if(clients.size() < 2)
                return;

            std::sort(clients.begin(), clients.end(),
                  [](Client *const &a, Client *const &b)
                  {
                      return a->GetPriority() > b->GetPriority();
                  });

            clients.at(0)->SetPriority(0);
            clients.at(1)->SetPriority(1);            
            manager->GetSelectedMonitor()->Sort();

            for(auto client : clients)
            {
                if(client->GetPriority() == 1)
                {
                    manager->SelectClient(client);
                    break;
                }
            }

         }},

        // Mod + Numeric 2
        {XK_KP_2, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            auto clients = manager->GetSelectedMonitor()->GetClients(manager->GetSelectedMonitor()->GetSelectedTag()->GetIndex() , FloatingStatus::FSNormal);

            if(clients.size() < 3)
                return;
            
            std::sort(clients.begin(), clients.end(),
                  [](Client *const &a, Client *const &b)
                  {
                      return a->GetPriority() > b->GetPriority();
                  });

            clients.at(0)->SetPriority(0);
            clients.at(2)->SetPriority(1);            
            manager->GetSelectedMonitor()->Sort();

            clients.at(0)->SetPriority(0);
            clients.at(1)->SetPriority(1);            
            manager->GetSelectedMonitor()->Sort();

            for(auto client : clients)
            {
                if(client->GetPriority() == 1)
                {
                    manager->SelectClient(client);
                    break;
                }
            }


         }},

        // Mod + Numeric 3
        {XK_KP_3, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            auto clients = manager->GetSelectedMonitor()->GetClients(manager->GetSelectedMonitor()->GetSelectedTag()->GetIndex() , FloatingStatus::FSNormal);

            if(clients.size() < 4)
                return;
            
            std::sort(clients.begin(), clients.end(),
                  [](Client *const &a, Client *const &b)
                  {
                      return a->GetPriority() > b->GetPriority();
                  });

            clients.at(0)->SetPriority(0);
            clients.at(3)->SetPriority(1);            
            manager->GetSelectedMonitor()->Sort();

            clients.at(0)->SetPriority(0);
            clients.at(1)->SetPriority(1);            
            manager->GetSelectedMonitor()->Sort();

            for(auto client : clients)
            {
                if(client->GetPriority() == 1)
                {
                    manager->SelectClient(client);
                    break;
                }
            }


         }},

        // Mod + Numeric 4
        {XK_KP_4, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            auto clients = manager->GetSelectedMonitor()->GetClients(manager->GetSelectedMonitor()->GetSelectedTag()->GetIndex() , FloatingStatus::FSNormal);

            if(clients.size() < 5)
                return;
            
            std::sort(clients.begin(), clients.end(),
                  [](Client *const &a, Client *const &b)
                  {
                      return a->GetPriority() > b->GetPriority();
                  });

            clients.at(0)->SetPriority(0);
            clients.at(4)->SetPriority(1);            
            manager->GetSelectedMonitor()->Sort();

            clients.at(0)->SetPriority(0);
            clients.at(1)->SetPriority(1);            
            manager->GetSelectedMonitor()->Sort();

            for(auto client : clients)
            {
                if(client->GetPriority() == 1)
                {
                    manager->SelectClient(client);
                    break;
                }
            }


         }},

        // Mod + Numeric 5
        {XK_KP_5, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            auto clients = manager->GetSelectedMonitor()->GetClients(manager->GetSelectedMonitor()->GetSelectedTag()->GetIndex() , FloatingStatus::FSNormal);

            if(clients.size() < 6)
                return;
            
            std::sort(clients.begin(), clients.end(),
                  [](Client *const &a, Client *const &b)
                  {
                      return a->GetPriority() > b->GetPriority();
                  });

            clients.at(0)->SetPriority(0);
            clients.at(5)->SetPriority(1);            
            manager->GetSelectedMonitor()->Sort();

            clients.at(0)->SetPriority(0);
            clients.at(1)->SetPriority(1);            
            manager->GetSelectedMonitor()->Sort();

            for(auto client : clients)
            {
                if(client->GetPriority() == 1)
                {
                    manager->SelectClient(client);
                    break;
                }
            }



         }},


         // Mod + Left
        {XK_Left, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            auto clients = manager->GetSelectedMonitor()->GetClients(manager->GetSelectedMonitor()->GetSelectedTag()->GetIndex() , FloatingStatus::FSNormal);

             if(clients.size() < 2)
                return;
          
            for(auto client : clients)
            {
                if(client->GetPriority() != 0)
                {
                    manager->SelectClient(client);
                    break;
                }
            }
            
            

         }},

          // Mod + Right
        {XK_Right, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
            auto clients = manager->GetSelectedMonitor()->GetClients(manager->GetSelectedMonitor()->GetSelectedTag()->GetIndex() , FloatingStatus::FSNormal);

             if(clients.size() < 2)
                return;
          
            for(auto client : clients)
            {
                if(client->GetPriority() == 0)
                {
                    manager->SelectClient(client);
                    break;
                }
            }
            
            

         }},

        // Mod + Space
        {XK_Shift_L, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             std::string layout = exec("setxkbmap -query | grep layout").substr(12, 2);

             if (layout == "de")
                 start("setxkbmap us");
             else if (layout == "us")
                 start("setxkbmap ir");
            else
                 start("setxkbmap de");

            manager->GetSelectedMonitor()->UpdateWidget("keyboard");
            manager->UpdateWidgets();

         }}};

}

#endif
