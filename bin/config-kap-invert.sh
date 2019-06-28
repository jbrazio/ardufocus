#!/usr/bin/env bash

cat << EOF >> ardufocus/config.h
#define UI_KAP_INVERT_BUTTON_LOGIC
EOF

exit 0
