# TG Window manager
This is my personal Debian/Arch window manager based-on X11. Even though I did my best to keep it configurable and customizable for any setup, but still there are some codes which are customized specifically for my hardware/monitor setup. So in case, you wanted to use it, make sure you make the necessary changes accordingly. 

Also any bug report, ideas or suggestions will be appreciated. Also, lots of features yet are missing which I try to do more on the weekends or in my spare time.

### Dependencies
The following libraries have been used :

1. [Xinerama](https://www.x.org/releases/current/doc/man/man3/Xinerama.3.xhtml) (To support multi-monitor setups)
2. [Xft](https://www.x.org/releases/current/doc/man/man3/Xft.3.xhtml) (Draw texts with a specific font and style)
3. [Google glog library](https://github.com/google/glog)
4. [Xephyr](https://www.x.org/archive/X11R7.5/doc/man/man1/Xephyr.1.html) (Virtual display emulator)
5. [Fontawesome Icon Pack - C++ Header](https://github.com/juliettef/IconFontCppHeaders)

### Installation

Note : Make sure you have the necessary dependencies mentioned in the previous section installed.

#### To start it manually with StartX :
```
git clone https://github.com/Foroughi/tgwm.git
cd tgwm
sudo make install
```

make sure you change your `.xinitrc` file to bootstrap the `tgwm` using the following command
```
exec tgwm
```
#### To start it with Display Manager :
Add a new Xsession desktop file `/usr/share/xsessions/tgwm.desktop` with the following content
```
[Desktop Entry]
Name=TGWM
Exec=tgwm
Type=XSession
```

### Uninstall

```
cd tgwm
sudo make uninstall
```

### Test

I have already setup a virtual display emullator using Xephyr. So in case you want to just have a look , you dont need to fully install it. Simply use `run.sh` file to start virtual display emulator

```
cd tgwm
./run.sh
```

### Special thanks to :

These are the two projects that showed me the ropes.

1. [Suckless DWM Project](https://dwm.suckless.org/)
2. [Basic_wm](https://github.com/jichu4n/basic_wm)
3. My friend Amin who helped me choose a suitable, yet most underrated name for the project.


[![Anurag's GitHub stats](https://github-readme-stats.vercel.app/api?username=foroughi)](https://github.com/anuraghazra/github-readme-stats)
