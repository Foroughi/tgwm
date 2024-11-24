#v1.0

apt update
apt install wget git ripgrep fd-find make curl libevent-dev ncurses-dev build-essential bison pkg-config  -y
 #install neovim
curl -LO https://github.com/neovim/neovim/releases/latest/download/nvim-linux64.tar.gz
rm -rf /opt/nvim
tar -C /opt -xzf nvim-linux64.tar.gz
echo export PATH=\"$PATH:/opt/nvim-linux64/bin\"  >> ~/.bashrc

echo export TERM=\"xterm-256color\" >>  ~/.bashrc

echo "cd /code" >> ~/.bashrc


apt install --yes make build-essential libx11-dev libgoogle-glog-dev xserver-xephyr xinit x11-apps xterm libxinerama-dev libxft-dev

echo "setup is done"
