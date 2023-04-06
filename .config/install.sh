sudo apt install -y xorg git wget software-properties-common awesome awesome-extra 
sudo add-apt-repository contrib
sudo add-apt-repository non-free
sudo apt update
sudo apt install -y nvidia-driver kitty

wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb
sudo dpkg -i google-chrome-stable_current_amd64.deb 
sudo apt -y --fix-broken install  
rm google-chrome-stable_current_amd64.deb 

sudo apt install -y cmake build-essential ranger compton rofi unzip alsa-utils nitrogen gnome-keyring network-manager-gnome htop
sudo apt install -y libimlib2-dev libncurses5-dev libx11-dev libxdamage-dev libxft-dev libxinerama-dev libxml2-dev libxext-dev libcurl4-openssl-dev liblua5.3-dev conky pavucontrol ntfs-3g firmware-realtek scrot

mkdir ~/.fonts

sudo apt install -y python3-pip
pip3 install requests

cp ~/.config/fonts/* ~/.fonts
fc-cache -f -v


mkdir -p ~/Downloads
mkdir -p ~/Pictures


sudo reboot