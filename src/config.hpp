
#include <vector>
#include <string>
#include "tag.hpp"
#include "widget.hpp"
#include "util.hpp"

#ifndef __CONFIG__
#define __CONFIG__

#define TOP_BAR_HEIGHT 33
#define GAP 7
#define TAGGAP 5
#define BORDER_WIDTH 3
#define HOTKEY Mod4Mask

#define TOPBAR_FG "#A0A0A0"
#define TOPBAR_BG 0x000000
#define TOPBAR_SELECTED_FG "#61afef"
#define CLIENT_NORMAL_BCOLOR 0x3B3B3B
#define CLIENT_SELECTED_BCOLOR 0x1E88E5
#define TOPBAR_FONT "monospace-7:style=Bold"
#define ICON_FONT "Font Awesome 6 Free:size=7:style=Solid"

#define ICON_FA_WIFI "\xef\x87\xab"         // U+f1eb
#define ICON_FA_MICROCHIP "\xef\x8b\x9b"    // U+f2db
#define ICON_FA_MEMORY "\xef\x94\xb8"       // U+f538
#define ICON_FA_COMPUTER "\xee\x93\xa5"     // U+e4e5
#define ICON_FA_CALENDAR "\xef\x84\xb3"     // U+f133
#define ICON_FA_CLOCK "\xef\x80\x97"        // U+f017
#define ICON_FA_VOLLEYBALL "\xef\x91\x9f"   // U+f45f
#define ICON_FA_VOLUME_HIGH "\xef\x80\xa8"  // U+f028
#define ICON_FA_VOLUME_LOW "\xef\x80\xa7"   // U+f027
#define ICON_FA_VOLUME_OFF "\xef\x80\xa6"   // U+f026
#define ICON_FA_VOLUME_XMARK "\xef\x9a\xa9" // U+f6a9

namespace CONFIG
{

    inline std::vector<Layouts> DefaultLayouts =
        {

            Layouts_Vertical,
            Layouts_Horizontal,

    };

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

    };

    inline std::vector<Tag *> Tags[] = {
        {new Tag(0, "", ICON_FA_COMPUTER),
         new Tag(1, "dev", ""),
         new Tag(2, "www", ""),
         new Tag(3, "term", ""),
         new Tag(4, "misc", "")},
        {new Tag(0, "www", ""),
         new Tag(1, "misc", "")}};

    inline std::vector<Widget *> Widgets = {
        new Widget(
            "time", Colors[0], ICON_FA_CLOCK,
            [](Widget *w)
            {
                return GetTime();
            },
            [](int button) {}),
        new Widget(
            "date", Colors[1], ICON_FA_CALENDAR,
            [](Widget *w)
            { return GetDate(); },
            [](int button) {}),
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
        new Widget(
            "network", Colors[3], ICON_FA_WIFI,
            [](Widget *w)
            { return ""; },
            [](int button) {}),
        new Widget(
            "cpu", Colors[4], ICON_FA_MICROCHIP,
            [](Widget *w)
            {
                return exec("cat /proc/stat |grep cpu |tail -1|awk '{print ($5*100)/($2+$3+$4+$5+$6+$7+$8+$9+$10)}'|awk '{print  100-$1}'").substr(0, 1) + "%";
            },

            [](int button) {}),
        new Widget(
            "memory", Colors[5], ICON_FA_MEMORY,
            [](Widget *w)
            {
                std::string memory = exec("free -h | grep Mem:");

                return memory.substr(16, 2) + "/" + memory.substr(27, 6);
            },
            [](int button) {})

    };
}
#endif