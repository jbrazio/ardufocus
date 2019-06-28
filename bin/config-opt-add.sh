#!/usr/bin/env bash

if [ -z "${1}" ]; then
  echo ERROR: Missing option name
  exit 1
fi

if [ -z "${2}" ]; then
  echo ERROR: Missing option value
  exit 1
fi

cat << EOF >> ardufocus/config.h
#define ${1} ${2}
EOF

exit 0
