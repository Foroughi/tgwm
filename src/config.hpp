

#ifndef __CONFIG__
#define __CONFIG__

#include <X11/Xutil.h>
#include <vector>
#include <string>
#include <cstring>
#include "tuple"
#include "./tag/tag.hpp"
#include "./widget/widget.hpp"
#include "./client/client.hpp"
#include "./monitor/monitor.hpp"
#include "./manager/manager.hpp"
#include "./util/util.hpp"
#include <glog/logging.h>

/*=====================================================================================================*/
/*=============================================== Configs =============================================*/
/*=====================================================================================================*/
#define TOP_BAR_HEIGHT 33
#define GAP 7
#define TAGGAP 5
#define BORDER_WIDTH 3
#define HOTKEY Mod4Mask
#define TAGS_HOVERABLE 0
#define TAGS_CLICKABLE 0
#define WIDGETS_HOVERABLE 0
#define WIDGETS_CLICKABLE 1
#define TOPBAR_FG "#A0A0A0"
#define TOPBAR_BG 0x000000
#define TOPBAR_SELECTED_FG "#61afef"
#define CLIENT_NORMAL_BCOLOR 0x3B3B3B
#define CLIENT_SELECTED_BCOLOR 0x1E88E5

/*=====================================================================================================*/
/*=============================================== Bootstrap =============================================*/
/*=====================================================================================================*/

inline std::function<void(Manager *)> BootstrapFunction = [](Manager *Manager)
{
    start("nitrogen --restore");
    start("compton");
};

/*=====================================================================================================*/
/*================================================ Fonts ==============================================*/
/*=====================================================================================================*/
#define TOPBAR_FONT "monospace-7:style=Bold"
#define TOPBAR_FONT_SUB "monospace-4:style=Bold"
#define ICON_FONT "Font Awesome 6 Free:size=7:style=Solid"

/*=====================================================================================================*/
/*================================================ Icons ==============================================*/
/*=====================================================================================================*/
#define ICON_FA_WIFI "\xef\x87\xab"
#define ICON_FA_MICROCHIP "\xef\x8b\x9b"
#define ICON_FA_MEMORY "\xef\x94\xb8"
#define ICON_FA_COMPUTER "\xee\x93\xa5"
#define ICON_FA_CALENDAR "\xef\x84\xb3"
#define ICON_FA_CLOCK "\xef\x80\x97"
#define ICON_FA_VOLLEYBALL "\xef\x91\x9f"
#define ICON_FA_VOLUME_HIGH "\xef\x80\xa8"
#define ICON_FA_VOLUME_LOW "\xef\x80\xa7"
#define ICON_FA_VOLUME_OFF "\xef\x80\xa6"
#define ICON_FA_VOLUME_XMARK "\xef\x9a\xa9"
#define ICON_FA_POWER_OFF "\xef\x80\x91"

namespace CONFIG
{

    /*=====================================================================================================*/
    /*============================================ Default Layouts ========================================*/
    /*=====================================================================================================*/
    inline std::vector<Layouts> DefaultLayouts =
        {

            Layouts_Vertical,
            Layouts_Horizontal,

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
        {new Tag(0, "", ICON_FA_COMPUTER),
         new Tag(1, "dev", ""),
         new Tag(2, "www", ""),
         new Tag(3, "term", ""),
         new Tag(4, "misc", "")},
        {new Tag(0, "www", ""),
         new Tag(1, "misc", "")}};

    /*=====================================================================================================*/
    /*================================================ Widgets ============================================*/
    /*=====================================================================================================*/
    inline std::vector<Widget *> Widgets = {

        // System Widget
        new Widget(
            "system", Colors[11], ICON_FA_POWER_OFF,
            [](Widget *w)
            {
                return "";
            },
            [](int button)
            {
                start("rofi -show power-menu -modi power-menu:\"~/.config/rofi/rofi-power-menu\" -config ~/.config/rofi/config.rasi");
            }),

        // Time Widget
        new Widget(
            "time", Colors[0], ICON_FA_CLOCK,
            [](Widget *w)
            {
                return GetTime();
            },
            [](int button) {}),

        // Date Widget
        new Widget(
            "date", Colors[1], ICON_FA_CALENDAR,
            [](Widget *w)
            { return GetDate(); },
            [](int button) {}),

        // Volume Widget
        new Widget(
            "volumn", Colors[2], ICON_FA_VOLUME_HIGH,
            [](Widget *w)
            {
                std::string volumn = exec("amixer sget Master | grep 'Left:' | awk -F'[][]' '{ print $2 }'");

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
            [](int button)
            {
                start("pavucontrol");
            }),

        // Network Widget
        new Widget(
            "network", Colors[3], ICON_FA_WIFI,
            [](Widget *w)
            { return ""; },
            [](int button) {

                start("nm-connection-editor");

            }),

        // Cpu Widget
        new Widget(
            "cpu", Colors[4], ICON_FA_MICROCHIP,
            [](Widget *w)
            {
                return exec("cat /proc/stat |grep cpu |tail -1|awk '{print ($5*100)/($2+$3+$4+$5+$6+$7+$8+$9+$10)}'|awk '{print  100-$1}'").substr(0, 1) + "%";
            },
            [](int button)
            {
                start("kitty htop");
            }),

        // Memory Widget
        new Widget(
            "memory", Colors[5], ICON_FA_MEMORY,
            [](Widget *w)
            {
                std::string memory = exec("free -h | grep Mem:");

                return memory.substr(16, 2) + "/" + memory.substr(27, 6);
            },
            [](int button) {})

    };

    /*=====================================================================================================*/
    /*=============================================== Bindings ============================================*/
    /*=====================================================================================================*/
    inline std::vector<std::tuple<int, int, std::function<void(Manager *, const XKeyEvent)>>> Keys = {

        // Mod + F1
        {XK_F1, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             LOG(INFO) << "======================================================";

             for (auto mon : manager->GetMonitors())
             {
                 LOG(INFO) << "Monitor (" + std::to_string(mon->GetScreen()) + "): (size :" + std::to_string(mon->GetSize().x) + ":" + std::to_string(mon->GetSize().y) + ") (loc :" + std::to_string(mon->GetLoc().x) + ":" + std::to_string(mon->GetLoc().y) + ")\n";

                 LOG(INFO) << "Tags :\n";
                 for (auto it : mon->GetTags())
                 {
                     LOG(INFO) << it->GetName() + "(" + std::to_string(it->GetIndex()) + ") (clients : " + std::to_string(mon->GetClients(it->GetIndex()).size()) + ") \n";
                 }

                 LOG(INFO) << "Clients :\n";
                 for (auto it : mon->GetClients(-1))
                 {
                     LOG(INFO) << "tag : (" + std::to_string(it->GetTagIndex()) + "): (size :" + std::to_string(it->GetSize().x) + ":" + std::to_string(it->GetSize().y) + ") (loc :" + std::to_string(it->GetLocation().x) + ":" + std::to_string(it->GetLocation().y) + ")\n";
                 }
             }

             LOG(INFO) << "======================================================";
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
         }},

        // Mod + 4
        {XK_4, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             manager->GetMonitor(0)->SelectTagByIndex(3);
         }},

        // Mod + Ctrl + 5
        {XK_5, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
             manager->MoveSelectedClient(manager->GetMonitor(0), 4);
             manager->SortAll();
         }},

        // Mod + 5
        {XK_5, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             manager->GetMonitor(0)->SelectTagByIndex(4);
         }},

        // Mod + Ctrl + 6
        {XK_6, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
             manager->MoveSelectedClient(manager->GetMonitor(1), 0);
             manager->SortAll();
         }},

        // Mod + 6
        {XK_6, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             manager->GetMonitor(1)->SelectTagByIndex(0);
         }},

        // Mod + Ctrl + 7
        {XK_7, HOTKEY | ControlMask, [](Manager *manager, const XKeyEvent &e)
         {
             manager->MoveSelectedClient(manager->GetMonitor(1), 1);
             manager->SortAll();
         }},

        // Mod + 7
        {XK_7, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             manager->GetMonitor(1)->SelectTagByIndex(1);
         }},

        // Mod + ~
        {XK_grave, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             start("rofi -no-lazy-grab -show drun -modi drun -config ~/.config/rofi/config.rasi");
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
             start("scrot -m -e 'mv $f /home/ali/Pictures/'");
         }},

        // Mod + Print
        {XK_Print, None, [](Manager *manager, const XKeyEvent &e)
         {
             start("scrot -mscrot -u -e 'mv $f /home/ali/Pictures/'");
         }},

        // Mod + Enter
        {XK_Return, HOTKEY, [](Manager *manager, const XKeyEvent &e)
         {
             start("kitty");
         }}

    };

}

#endif