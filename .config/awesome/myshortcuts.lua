local gears = require("gears")
local awful = require("awful")
local naughty = require("naughty")
local menubar = require("menubar")


local modkey = "Mod4"
local altkey = "Alt"
local ctrlkey = "Control"
local hotkeys_popup = require("awful.hotkeys_popup")
local home = os.getenv("HOME")
local terminal = "kitty"
local editor = "nano"
local file_manager = terminal .. " ranger"
local browser = "google-chrome-stable"

local rofi_dir = home .. "/.config/awesome/rofi/"

menubar.show_categories = false

function spawn_and_notify(command, message, message_cmd)
   return function()
      awful.spawn(command)
      if message_cmd then
         os.execute("sleep 0.05") -- `volume -get` is slightly faster than `volume -set`
         awful.spawn.easy_async(message_cmd, function(stdout, stderr, reason, exit_code)
            naughty.notify { text = message .. string.gsub(stdout, "\n", "") }
         end)
      else
         naughty.notify { text = message }
      end
   end
end

-- {{{ Key bindings
local globalkeys = gears.table.join(
   awful.key({ modkey, }, "/", hotkeys_popup.show_help,
      { description = "show help", group = "awesome" }),

   awful.key({ modkey, }, "Left",
      function()
         awful.client.focus.bydirection("left")
      end,
      { description = "view previous", group = "client" }),

   awful.key({ modkey, }, "Right",
      function()
         awful.client.focus.bydirection("right")
      end,
      { description = "view previous", group = "client" }),

   awful.key({ modkey, }, "Up",
      function()
         awful.client.focus.bydirection("up")
      end,
      { description = "view previous", group = "client" }),

   awful.key({ modkey, }, "Down",
      function()
         awful.client.focus.bydirection("down")
      end,
      { description = "view previous", group = "client" }),

   -- awful.key({ modkey, }, "Escape", awful.tag.history.restore,
   -- {description = "go back", group = "tag"}),




   awful.key({ modkey }, "Tab",
      function()
         awful.spawn("rofi -show window -config " .. rofi_dir .. "config.rasi")
      end,
      { description = "show all windows from all workspaces", group = "awesome" }),


   awful.key({ modkey, ctrlkey }, "j", function() awful.screen.focus_relative(1) end,
      { description = "focus the next screen", group = "screen" }),

   awful.key({ modkey, ctrlkey }, "k", function() awful.screen.focus_relative(-1) end,
      { description = "focus the previous screen", group = "screen" }),

   awful.key({ modkey, }, "u", awful.client.urgent.jumpto,
      { description = "jump to urgent client", group = "client" }),


   -- Standard program
   awful.key({ modkey, }, "Return", function() awful.spawn(terminal) end,
      { description = "open a terminal", group = "launcher" }),


   awful.key({ modkey, "Control" }, "r", awesome.restart,
      { description = "reload awesome", group = "awesome" }),




   awful.key({ modkey }, "b", function() awful.spawn(browser) end,
      { description = "launch Browser", group = "launcher" }),


   awful.key({ modkey, }, "e", function() awful.spawn(file_manager) end,
      { description = "launch filemanager", group = "launcher" }),


   awful.key({ modkey, "Shift" }, "q", awesome.quit,
      { description = "quit awesome", group = "awesome" }),

   awful.key({ modkey, }, "`",
      function() awful.spawn("rofi -no-lazy-grab -show drun -modi drun -config " .. rofi_dir .. "config.rasi") end,
      { description = "launch rofi", group = "launcher" }),

   awful.key({ }, "Pause",
      function() awful.spawn("rofi -show power-menu -modi power-menu:" .. rofi_dir .. "rofi-power-menu -config " .. rofi_dir .. "config.rasi") end,
      { description = "Shutdown menu", group = "launcher" }),


   awful.key({}, "Print", function() awful.spawn("scrot -u -e 'mv $f /home/ali/Pictures/'") end,
      { description = "capture a screenshot", group = "screenshot" }),

   awful.key({ ctrlkey }, "Print", function() awful.spawn("scrot -m -e 'mv $f /home/ali/Pictures/'") end,
      { description = "capture a screenshot of active window", group = "screenshot" }),

   -- awful.key({}, "XF86AudioMute", function() awful.spawn("amixer -q set Master toggle") end,
   --    { description = "Mute/Unmute master volumn", group = "Sound" }),

   awful.key({ }, "XF86AudioMute", spawn_and_notify("amixer -q set Master toggle", "Mute/Unmute toggled"),
      { description = "Mute/Unmute master volumn", group = "Sound" }),

   awful.key({}, "XF86AudioLowerVolume", function() awful.spawn("amixer set Master 5%-") end,
      { description = "Lower master volumn", group = "Sound" }),

   awful.key({}, "XF86AudioRaiseVolume", function() awful.spawn("amixer set Master 5%+") end,
      { description = "Raise master volumn", group = "Sound" }),



   awful.key({ modkey, ctrlkey }, "Up", function() awful.client.incwfact(0.05) end,
      { description = "increase client size vertically", group = "client" }),

   awful.key({ modkey, ctrlkey }, "Down", function() awful.client.incwfact(-0.05) end,
      { description = "decrease client size vertically", group = "client" }),

   awful.key({ modkey, ctrlkey }, "Right", function() awful.tag.incmwfact(0.05) end,
      { description = "increase master width factor", group = "layout" }),

   awful.key({ modkey, ctrlkey }, "Left", function() awful.tag.incmwfact(-0.05) end,
      { description = "decrease master width factor", group = "layout" })


)


clientkeys = gears.table.join(

   awful.key({ modkey, }, "c", function(c) c:kill() end,
      { description = "close", group = "client" })




)


-- clientbuttons = gears.table.join(
--    awful.button({}, 1, function(c) client.focus = c;
--       c:raise()
--       mymainmenu:hide()
--    end),
--    awful.button({ modkey }, 1, awful.mouse.client.move),
--    awful.button({ ctrlkey }, 1, awful.mouse.client.resize)
-- )

-- Bind all key numbers to tags.
-- Be careful: we use keycodes to make it work on any keyboard layout.

local np_map = { 87, 88, 89, 83, 84, 85, 79, 80, 81 }
for i = 1, 9 do
   globalkeys = gears.table.join(
      globalkeys,

      -- This should map on the top row of your keyboard, usually 1 to 9.
      -- View tag only.
      awful.key({ modkey }, "#" .. i + 9,
         function()

            local tag = screen[1].tags[i]

            if i > 4 then

               tag = screen[2].tags[i - 4]
            end

            if tag then
               tag:view_only()
            end

         end,
         { description = "view tag #" .. i, group = "tag" }),

      -- Move client to tag.
      awful.key({ modkey, "Shift" }, "#" .. i + 9,
         function()

            local tag = screen[1].tags[i]

            if i > 4 then
               tag = screen[2].tags[i - 4]
            end

            if client.focus then
               client.focus:move_to_tag(tag)
            end
         end,
         { description = "move focused client to tag #" .. i, group = "tag" })
   )
end
return globalkeys
