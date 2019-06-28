#!/usr/bin/env bash

if [ -z "${1}" ]; then
  echo ERROR: No motor number selected
  exit 1
fi

cat << EOF >> ardufocus/config.h
#define MOTOR${1}_USE_ULN2003_DRIVER
#define MOTOR${1}_PINOUT 2, 3, 4, 5
EOF

exit 0
