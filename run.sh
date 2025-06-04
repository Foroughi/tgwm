set -e

make DEBUGMODEFLAG="-DTGWMDEBUG"

XEPHYR=$(whereis -b Xephyr | cut -f2 -d' ')
xinit ./xinitrc -- "$XEPHYR"  :100 -ac -screen 1400x1000 -host-cursor
