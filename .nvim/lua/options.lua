local opt = vim.opt -- aliasing

-- line number
opt.number = true

-- tabs & intendation
opt.tabstop = 4
opt.shiftwidth = 4
opt.expandtab = true
opt.autoindent = true

-- search
opt.ignorecase = true
opt.smartcase = true

-- appearance
opt.background = "dark"
opt.termguicolors = true

opt.swapfile = false

vim.api.nvim_set_option("clipboard", "unnamed")
opt.mouse = ""

opt.wrap = false


