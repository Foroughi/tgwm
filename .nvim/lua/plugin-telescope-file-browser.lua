return {

    config = {
        "nvim-telescope/telescope-file-browser.nvim",
        dependencies = { "nvim-telescope/telescope.nvim", "nvim-lua/plenary.nvim" }        
    },
    setup = function()
        local fb_actions = require "telescope".extensions.file_browser.actions
        require("telescope").setup {
        extensions = {
          file_browser = {
            --theme = "ivy",
            -- disables netrw and use telescope-file-browser in its place
            hijack_netrw = true,
            initial_mode="normal",
            mappings = {
              ["n"] = {                  
                  ["<C-c>"] = fb_actions.create_from_prompt,
                  ["<C-r>"] = fb_actions.rename,
                  ["<C-x>"] = fb_actions.move,
                  ["<C-y>"] = fb_actions.copy,
                  ["<C-d>"] = fb_actions.remove,                  
                  ["<C-p>"] = fb_actions.goto_parent_dir,                
                  ["<C-w>"] = fb_actions.goto_cwd,                            
                  ["<C-h>"] = fb_actions.toggle_hidden,                            
              },
            
            },
          },
        },
      }
      -- To get telescope-file-browser loaded and working with telescope,
      -- you need to call load_extension, somewhere after setup function:
      require("telescope").load_extension "file_browser"
    end

}
