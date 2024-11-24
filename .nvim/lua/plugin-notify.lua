return {
    
    config = {
        "rcarriga/nvim-notify",
        event = "VeryLazy"        
    },
    setup = function()
        vim.notify = require("notify")
    end

}
