#!/usr/bin/env bash

cat << EOF >> ardufocus/config.h
#define UI_KAP_FWD_BUTTON_LED_PIN 13
#define UI_KAP_BWD_BUTTON_LED_PIN 13
#define UI_KAP_SWT_BUTTON_LED_PIN 13
EOF

exit 0
