#!/usr/bin/env bash

cat << EOF >> ardufocus/config.h
#define UI_KAP_SWT_BUTTON_PIN 18
EOF

exit 0
