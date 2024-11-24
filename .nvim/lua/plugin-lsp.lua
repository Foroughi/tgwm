local kind_icons = {
  Text = '  ',
  Method = '  ',
  Function = '  ',
  Constructor = '  ',
  Field = '  ',
  Variable = '  ',
  Class = '  ',
  Interface = '  ',
  Module = '  ',
  Property = '  ',
  Unit = '  ',
  Value = '  ',
  Enum = '  ',
  Keyword = '  ',
  Snippet = '  ',
  Color = '  ',
  File = '  ',
  Reference = '  ',
  Folder = '  ',
  EnumMember = '  ',
  Constant = '  ',
  Struct = '  ',
  Event = '  ',
  Operator = '  ',
  TypeParameter = '  ',
}
  
local has_words_before = function()
  unpack = unpack or table.unpack
  local line, col = unpack(vim.api.nvim_win_get_cursor(0))
  return col ~= 0 and vim.api.nvim_buf_get_lines(0, line - 1, line, true)[1]:sub(col, col):match("%s") == nil
end
	
local setup_tools = function()

    local luasnip = require("luasnip")
    require("luasnip.loaders.from_vscode").lazy_load()
    local cmp_autopairs = require('nvim-autopairs.completion.cmp')
    local cmp = require('cmp')

    cmp.setup {
        sources = {
        
          {name = 'nvim_lsp', keyword_length = 1},
          {name = 'buffer', keyword_length = 3},
          
        },
        snippet = {
            expand = function(args)
              luasnip.lsp_expand(args.body)
            end
        },
       
		formatting = {
			fields = {'kind' , 'abbr' ,'menu'},
			format = function(entry, vim_item)
			 
				vim_item.kind = string.format(' %s', kind_icons[vim_item.kind])

				return vim_item
			 
			end
		},
       mapping = cmp.mapping.preset.insert({
            --['<C-Space>'] = cmp.mapping.complete(),
           ['<CR>'] = cmp.mapping.confirm({select= true}),

            ["<Tab>"] = cmp.mapping(function(fallback)

              if cmp.visible() then
                cmp.select_next_item()
              elseif luasnip.jumpable(1) then
                luasnip.jump(1)
              else
                fallback()
              end
            end, { "i", "s" }),

            ["<S-Tab>"] = cmp.mapping(function(fallback)
              if cmp.visible() then
                cmp.select_prev_item()
              elseif luasnip.jumpable(-1) then
                luasnip.jump(-1)
              else
                fallback()
              end
            end, { "i", "s" }),
        }),

        window = {

            documentation = {
              winhighlight = "Normal:Pmenu,FloatBorder:Pmenu,Search:None",
              col_offset = 0,			  
              side_padding = 0,
            },
            completion = {
              winhighlight = "Normal:Pmenu,FloatBorder:Pmenu,Search:None",
              col_offset = 0,
              side_padding = 0,
            },
          }
    }


     vim.api.nvim_create_autocmd('LspAttach', {
        group = vim.api.nvim_create_augroup('lsp-attach-format', { clear = true }),
        -- This is where we attach the autoformatting for reasonable clients
        callback = function(args)
            local client_id = args.data.client_id
            local client = vim.lsp.get_client_by_id(client_id)
            local bufnr = args.buf
            if not client.server_capabilities.documentFormattingProvider then
                return
            end

            local opts = { buffer = bufnr, noremap = true, silent = true }
            vim.keymap.set('n', '<space>mD', vim.lsp.buf.declaration, opts)
            vim.keymap.set('n', '<space>md', vim.lsp.buf.definition, opts)
            vim.keymap.set('n', '<space>mh', vim.lsp.buf.hover, opts)
            vim.keymap.set('n', '<space>mi', vim.lsp.buf.implementation, opts)
            --vim.keymap.set('n', '<space>dK', vim.lsp.buf.signature_help, opts)
            vim.keymap.set('n', '<space>mwa', vim.lsp.buf.add_workspace_folder, opts)
            vim.keymap.set('n', '<space>mwr', vim.lsp.buf.remove_workspace_folder, opts)
            vim.keymap.set('n', '<space>mwl', function()
              print(vim.inspect(vim.lsp.buf.list_workspace_folders()))
            end, opts)
            --vim.keymap.set('n', '<space>dD', vim.lsp.buf.type_definition, opts)
            vim.keymap.set('n', '<space>mr', vim.lsp.buf.rename, opts)
            vim.keymap.set('n', '<space>mR', vim.lsp.buf.references, opts)
            vim.keymap.set('n', '<space>me', vim.diagnostic.open_float, opts)
            vim.keymap.set('n', '<space>m[', vim.diagnostic.goto_prev, opts)
            vim.keymap.set('n', '<space>m]', vim.diagnostic.goto_next, opts)
            vim.keymap.set('n', '<space>mq', vim.diagnostic.setloclist, opts)
            vim.api.nvim_create_autocmd('BufWritePre', {

                buffer = bufnr,
                callback = function()
                    if not format_is_enabled then
                        return
                    end
                    vim.lsp.buf.format {
                        async = false,
                        filter = function(c)
                            return c.id == client.id
                        end,
                    }
                end,
            })
        end,
    })
	
    cmp.event:on(
       'confirm_done',
	cmp_autopairs.on_confirm_done()
    )
		
end

return {

    config = {
        "neovim/nvim-lspconfig",
        dependencies = {
            "hrsh7th/nvim-cmp",
            "hrsh7th/cmp-nvim-lsp",
            {
                "L3MON4D3/LuaSnip",
                build = "make install_jsregexp"
            }

        }
    },
    setup = function()

        local lspconfig = require('lspconfig')

        local servers = {
            "ts_ls",
            "clangd",
            --"lua-language-server"
        }
        lspconfig.ts_ls.setup {}
        --lspconfig.lua.setup {}
        lspconfig.clangd.setup {
            cmd = { 'clangd', '--background-index', '--compile-commands-dir', 'D:/systemc/excersies/build' },
            init_options = {
                clangdFileStatus = true,
                clangdSemanticHighlighting = true
            },
            filetypes = { 'c', 'cpp', 'h' },
            root_dir = function() vim.fn.getcwd() end,
            settings = {
                ['clangd'] = {
                    ['compilationDatabasePath'] = 'build',
                    ['fallbackFlags'] = { '-std=c++17' }
                }
            }
        }

        for _, lsp in ipairs(servers) do
            lspconfig[lsp].setup {
                -- on_attach = my_custom_on_attach,
                capabilities = require("cmp_nvim_lsp").default_capabilities(),
            }
        end

        setup_tools();
    end

}
