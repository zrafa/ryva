# ryva


sudo apt install v4l-utils
v4l2-ctl -D
v4l2-ctl --list-formats-ext
v4l2-ctl -V
v4l2-ctl -stream-mmap=3 --stream-count=1000 --stream-to=/dev/null



