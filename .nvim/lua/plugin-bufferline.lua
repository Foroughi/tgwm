return {

    config = {
        'akinsho/bufferline.nvim',
        version = "*",
        dependencies = { 
            "nvim-tree/nvim-web-devicons"           
        }
    },
    setup = function()

        local bufferline = require("bufferline");
        bufferline.setup {
            options = {
                mode = "tabs",
                seperator_style = "slant",
                style_preset = "",
                themeable = true, bufferline.style_preset.default,
                indicator_icon = "▎",
                modified_icon = "●",
                buffer_close_icon = "",
                close_icon = "",
                left_trunc_marker = "",
                right_trunc_marker = "",
                numbers = "none",
                max_name_length = 15,
                max_prefix_length = 6,
                diagnostics = "nvim_lsp",
                show_buffer_icons = true,
                show_buffer_close_icons = false,
                show_close_icon = false,
                persist_buffer_sort = true,
                enforce_regular_tabs = true,
                diagnostics_indicator = function(count, level)
                    local icon = level:match("error") and "" or ""
                    return icon .. count
                end,
            },
        }


    end

}
