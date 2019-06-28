#!/usr/bin/env bash

if [ -z "${1}" ]; then
  echo ERROR: No motor number selected
  exit 1
fi

cat << EOF >> ardufocus/config.h
#define MOTOR${1}_MAX_SPEED 250
#define MOTOR${1}_MIN_SPEED 10
EOF

exit 0
