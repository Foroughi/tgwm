local awful = require("awful")
local beautiful = require("beautiful")

myrules = {

  { rule = { },
    properties = { border_width = beautiful.border_width,
      border_color = beautiful.border_normal,
      focus = awful.client.focus.filter,
      raise = true,
      keys = clientkeys,
      buttons = clientbuttons,
      size_hints_honor = false, -- Remove gaps between terminals
      screen = awful.screen.preferred,
      callback = awful.client.setslave,
      placement = awful.placement.no_overlap + awful.placement.no_offscreen
    }
  },

  { rule_any = { type = { "normal", "dialog" } },
    properties = { titlebars_enabled = true , floating =true }
  },

  { rule = { class = "Google-chrome" }, properties = { screen = 2, tag = "www", floating = false } },
  { rule = { class = "Code" }, properties = { screen = 1, tag = "dev", floating = false } },
  { rule = { class = "kitty" }, properties = { screen = 1, tag = "term", floating = false } },

  { rule = { class = "feh" }, properties = { floating = false } },



}

return myrules
