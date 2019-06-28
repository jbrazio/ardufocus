#!/usr/bin/env bash

cat << EOF >> ardufocus/config.h
#define USE_UI_KAP
#define UI_KAP_ADC_CHANNEL 1
#define UI_KAP_FWD_BUTTON_PIN 2
#define UI_KAP_BWD_BUTTON_PIN 3
EOF

exit 0
