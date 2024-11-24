return {

    config = {
        "folke/which-key.nvim",
        event = "VeryLazy",
        init = function()
            vim.o.timeout = true
            vim.o.timeoutlen = 0
        end,
        config = true,
        opts = {
            preset = "classic",
            key_labels = {

                ["<leader>"] = "SPACE",
                ["<space>"] = "SPACE",
                ["<esc>"] = "ESC",
                ["<CR>"] = "ENTER",
                ["<bs>"] = "BS",
                ["<tab>"] = "TAB",
            },
            window = {
                border = "single",        -- none, single, double, shadow
                position = "bottom",      -- bottom, top
                margin = { 0, 1, 1, 0.75 },  -- extra window margin [top, right, bottom, left]. When between 0 and 1, will be treated as a percentage of the screen size.
                padding = { 1, 1, 1, 1 }, -- extra window padding [top, right, bottom, left]
                winblend = 20,             -- value between 0-100 0 for fully opaque and 100 for fully transparent
                zindex = 1000,            -- positive value to position WhichKey above other floating windows.
            },
            layout = {
                height = { min = 4, max = 65 }, -- min and max height of the columns
                width = { min = 100, max =  100 }, -- min and max width of the columns
                spacing = 0,                    -- spacing between columns
                align = "center",                 -- align columns left, center or right
            },
            ignore_missing = false,
        }
    },
    setup = function()
            local wk = require("which-key")

        wk.add({
            { "<leader>f", group = "Files" },
            { "<leader>fb", "<cmd>Telescope buffers<cr>", desc = "List of opened windows" },
            { "<leader>fd", "<cmd>Telescope diagnostics<cr>", desc = "View LSP diagnostics" },
            { "<leader>ff", "<cmd>Telescope find_files<cr>", desc = "Find a file in current directory" },
            { "<leader>fg", "<cmd>Telescope live_grep<cr>", desc = "Find in files" },
            { "<leader>fh", "<cmd>Telescope help_tags<cr>", desc = "Help" },
            { "<leader>ft", "<cmd>Telescope file_browser path=%:p:help |select_buffer=true<cr>|", desc = "Show file tree" },
            { "<leader>m", group = "Diagnostics" },
            { "<leader>mD", desc = "View declaration" },
            { "<leader>mR", desc = "Refrences" },
            { "<leader>m[", desc = "Prevoius diagnostic error" },
            { "<leader>m]", desc = "Next diagnostic error" },
            { "<leader>md", desc = "View definition" },
            { "<leader>me", desc = "Floating diagnostics win" },
            { "<leader>mh", desc = "Hover" },
            { "<leader>mi", desc = "Go to implementation" },
            { "<leader>mq", desc = "Diagnostic error list" },
            { "<leader>mr", desc = "Rename" },
            { "<leader>mwa", desc = "Add workspace folder" },
            { "<leader>mwl", desc = "List workspace folders" },
            { "<leader>mwr", desc = "Remove workspace folder" },
            { "<leader>t", group = "Tab navigation" },
            { "<leader>tQ", "<cmd>quitall<cr>", desc = "Close all" },
            { "<leader>th", "<cmd>tabprev<cr>", desc = "Move to previouse tab" },
            { "<leader>tl", "<cmd>tabnext<cr>", desc = "Move to next tab" },
            { "<leader>tn", "<cmd>tabnew<cr>", desc = "New tab" },
            { "<leader>tq", "<cmd>q<cr>", desc = "Close current tab" },
            { "<leader>w", group = "Window navigation" },
            { "<leader>wQ", "<cmd>quitall<cr>", desc = "Close all" },
            { "<leader>wh", "<cmd>wincmd h<cr>", desc = "Move to right window" },
            { "<leader>wj", "<cmd>wincmd j<cr>", desc = "Move to down window" },
            { "<leader>wk", "<cmd>wincmd k<cr>", desc = "Move to top window" },
            { "<leader>wl", "<cmd>wincmd l<cr>", desc = "Move to left window" },
            { "<leader>wn", group = "New file" },
            { "<leader>wnh", "<cmd>hnew<cr>", desc = "Create new window horizontally" },
            { "<leader>wnn", "<cmd>new<cr>", desc = "Create new window" },
            { "<leader>wnv", "<cmd>vnew<cr>", desc = "Create new window vertically" },
            { "<leader>wq", "<cmd>q<cr>", desc = "Close current window" },
            { "<leader>wr", group = "Resize" },
            { "<leader>wrh", "<cmd>vert resize -10<cr>", desc = "Resize to left" },
            { "<leader>wrj", "<cmd>resize +5<cr>", desc = "Resize to down" },
            { "<leader>wrk", "<cmd>resize -5<cr>", desc = "Resize to up" },
            { "<leader>wrl", "<cmd>vert resize +10<cr>", desc = "Resize to right" },
            { "<leader>ws", group = "Split" },
            { "<leader>wsh", "<cmd>split<cr>", desc = "Split horizontally" },
            { "<leader>wsv", "<cmd>vsplit<cr>", desc = "Split vertically" },
      })
    end

}

