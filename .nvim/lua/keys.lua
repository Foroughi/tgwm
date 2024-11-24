vim.g.mapleader = ' '
local opts = { noremap = true, silent = true }

vim.keymap.set({'c' , 'i'}, '<C-j>', '<Down>', opts)
vim.keymap.set({'c' , 'i'}, '<C-k>', '<Up>', opts)
vim.keymap.set({'c' , 'i'}, '<C-l>', '<Right>', opts)
vim.keymap.set({'c' , 'i'}, '<C-h>', '<Left>', opts)

vim.keymap.set({'c' , 'v' , 'c' , 'i'}, '<c-s>' , "<cmd>:write<cr>")

-- to get used to vim key layout
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<Up>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<Down>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<Left>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<Right>', '<Nop>', opts)

vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<c-Up>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<c-Down>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<c-Left>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<c-Right>', '<Nop>', opts)

vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<s-Up>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<s-Down>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<s-Left>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<s-Right>', '<Nop>', opts)

vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<PageUp>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<PageDown>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<c-PageUp>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<c-PageDown>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<s-PageUp>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<s-PageDown>', '<Nop>', opts)

vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<Home>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<End>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<s-Home>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<s-End>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<c-Home>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<c-End>', '<Nop>', opts)

vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<Insert>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<Delete>', '<Nop>', opts)

vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<c-Insert>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<c-Delete>', '<Nop>', opts)

vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<s-Insert>', '<Nop>', opts)
vim.keymap.set({'n', 'v' , 'c' , 'i'}, '<s-Delete>', '<Nop>', opts)
