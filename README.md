# TG Window manager
This is my personal Debian window manager based-on X11 and written in C++. Even though I did my best to keep configurable or customizable for any setup, but still it's still there are some codes which are customized specifically for my hardware/monitor setup. So in case, you wanted to use it, make sure you make the necessary changes accordingly. 

Also, it's safe to say that since this repository is still under development, there is a good chance of encountering bugs or failures. For the same reason, lots of features yet are missing. But any bug report will be appreciated.

### Dependencies
The following libraries have been used :

1. [Xinerama](https://www.x.org/releases/current/doc/man/man3/Xinerama.3.xhtml) (To support multi-monitors setups)
2. [Xft](https://www.x.org/releases/current/doc/man/man3/Xft.3.xhtml) (Draw texts with a specific font and style)
3. [Google glog library](https://github.com/google/glog)
4. [Xephyr](https://www.x.org/archive/X11R7.5/doc/man/man1/Xephyr.1.html) (Virtual display emulator)

### Installation

Note : Make sure you have the necessary dependencies installed mentioned in the previous section.

```
git clone https://github.com/Foroughi/tgwm.git
cd tgwm
sudo make install
```

make usre you change your `.xinit` files and run the `tgwm` using the following command
```
exec tgwm
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

