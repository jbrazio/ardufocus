#!/usr/bin/env bash

if [ -z "${1}" ]; then
  echo ERROR: No motor number selected
  exit 1
fi

cat << EOF >> ardufocus/config.h
#define MOTOR${1}_INVERT_DIRECTION
EOF

exit 0
