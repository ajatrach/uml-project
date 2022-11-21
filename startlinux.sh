./linux \
  root=/dev/root \
  rootfstype=hostfs \
  rootflags=$HOME/prefix/uml-demo \
  rw \
  mem=64M \
  init=/bin/sh
