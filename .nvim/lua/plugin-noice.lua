return {

  config = {
    "folke/noice.nvim",
    event = "VeryLazy",
    dependencies = {
      -- if you lazy-load any plugin below, make sure to add proper `module="..."` entries
      "MunifTanjim/nui.nvim",
      -- OPTIONAL:
      --   `nvim-notify` is only needed, if you want to use the notification view.
      --   If not available, we use `mini` as the fallback
      --"rcarriga/nvim-notify",
    },

    opts = {
      cmdline = {
          enabled = true,
          view = "cmdline"
      },
      lsp = {
        progress = {
          enabled = true,

          format = "lsp_progress",
          format_done = "lsp_progress_done",
          throttle = 1000 / 30, -- frequency to update lsp progress message
          view = "mini",
        },
        override = {
          ["vim.lsp.util.convert_input_to_markdown_lines"] = false,
          ["vim.lsp.util.stylize_markdown"] = false,
          ["cmp.entry.get_documentation"] = false,
        },
        hover = {
          enabled = true,
          silent = false,
          view = nil, -- when nil, use defaults from documentation

          opts = {},
        },
        signature = {
          enabled = true,
		  silent = false,
          auto_open = {
            enabled = true,
            trigger = true, -- Automatically show signature help when typing a trigger character from the LSP
            luasnip = true, -- Will open signature help when jumping to Luasnip insert nodes
            throttle = 50, -- Debounce lsp signature help request by 50ms
          },
          view = "hover", -- when nil, use defaults from documentation
          ---@type NoiceViewOptions
          opts = {
            relative = "cursor",
            position = {
              row = 2,
              col = 2,
            },
            size = {
              --min_width = 60,
              width = "auto",
              height = "auto",
            },
            border = {
			  style = "none",
              padding = { 1, 1 },
            },
          }, -- merged with defaults from documentation
        },
        message = {
          -- Messages shown by lsp servers
          enabled = true,
          opts = {},
        },
        -- defaults for hover and signature help
        documentation = {
          view = "hover",
          ---@type NoiceViewOptions
          opts = {
            lang = "markdown",
            replace = true,
            render = "plain",
            format = { "{message}" },
            win_options = { concealcursor = "n", conceallevel = 3 },
          },
        },

      },

      presets = {
        bottom_search = true,               -- use a classic bottom cmdline for search
        command_palette = false,            -- position the cmdline and popupmenu together
        long_message_to_split = true,       -- long messages will be sent to a split
        inc_rename = true,                  -- enables an input dialog for inc-rename.nvim
        lsp_doc_border = true,              -- add a border to hover docs and signature help
      },
      views = {
        -- Clean cmdline_popup + palette
        cmdline_popup = {
          position = {
            row = 10,
            col = "50%",
          },
          size = {
            --min_width = 60,
            width = 60,
            height = "auto",
          },
          border = {
            text = {
              top = "Search",
              top_align = "center",
            },
          },
          win_options = {
            winhighlight = { NormalFloat = "NormalFloat", FloatBorder = "FloatBorder" },
          },
        },
        cmdline_popupmenu = {
          size = {
            width = 60,
            height = "auto",
            max_height = 20,
          },
        },
        hover = {
          border = {
            style = "single",
          },
        },
        confirm = {
          border = {
            style = "single",
          },
        },
        popup = {
          border = {
            style = "single",
          },
        },
      },
    },


  },
  setup = function()
    require("noice")
  end

}
