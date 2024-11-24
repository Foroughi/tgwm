return {

    config = {
        'akinsho/toggleterm.nvim',
        version = "*",
        config = true
        
    }
    ,
    setup = function()
        require("toggleterm").setup({
            -- size can be a number or function which is passed the current terminal
            size = 20,
            open_mapping = [[<c-\>]],
            hide_numbers = true,
            shade_terminals = true,

            start_in_insert = true,
            insert_mappings = true,      -- whether or not the open mapping applies in insert mode
            terminal_mappings = true,    -- whether or not the open mapping applies in the opened terminals
            direction = 'horizontal',

            --direction = 'vertical' | 'horizontal' | 'tab' | 'float',
            close_on_exit = true, -- close the terminal window when the process exits
            -- Change the default shell. Can be a string or a function returning a string
            shell = vim.o.shell,
            auto_scroll = true, -- automatically scroll to the bottom on terminal output
            -- This field is only relevant if direction is set to 'float'

        })


        function _G.set_terminal_keymaps()
            local opts = { noremap = true }
            vim.api.nvim_buf_set_keymap(0, 't', '<esc>', [[<C-\><C-n>]], opts)           
        end

        vim.cmd('autocmd! TermOpen term://* lua set_terminal_keymaps()')
    end

}
