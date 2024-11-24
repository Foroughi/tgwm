return {

    config = {
        "lukas-reineke/indent-blankline.nvim",
        --event = "BufReadPost",
        main = "ibl",
        opts = {}
    },
    setup = function()
        require("ibl").setup{}
    end

}
