return {

    config = {
        {
            'nvim-telescope/telescope.nvim',
            tag = '0.1.6',
            -- or                              , branch = '0.1.x',
            dependencies = { 
                'nvim-lua/plenary.nvim'
                
            }
        }
    },
    setup = function()

        local telescope = require("telescope")
        telescope.setup({
            pickers = {
                live_grep = {
                    initial_mode="normal",
                    file_ignore_patterns = { 'node_modules', '.git', '.venv' , '.nvim'},
                    additional_args = function(_)
                        return { "--hidden" }
                    end
                },
                find_files = {
                    initial_mode="normal",
                    file_ignore_patterns = { 'node_modules', '.git', '.venv' , '.nvim'},
                    hidden = true
                }

            }           
        })    

        local actions = require("telescope.actions")
        local builtin = require('telescope.builtin')               
        vim.keymap.set('n', '<leader>ff', builtin.find_files, {})
        vim.keymap.set('n', '<leader>fg', builtin.live_grep, {})
        vim.keymap.set('n', '<leader>fb', builtin.buffers, {})
        vim.keymap.set('n', '<leader>fh', builtin.help_tags, {})
        vim.keymap.set('n', '<leader>fd', builtin.diagnostics , {})

        
    end

}
