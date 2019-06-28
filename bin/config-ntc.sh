#!/usr/bin/env bash
cat << EOF >> ardufocus/config.h
#define NTC_ADC_CHANNEL          0
#define NTC_NOMINAL_TEMP      25.0F
#define NTC_BCOEFFICIENT    3950.0F
#define NTC_NOMINAL_VAL    10000.0F
#define NTC_RESISTOR_VAL   10000.0F

#define NTC_MIN_RAW_VALUE  50
#define NTC_MAX_RAW_VALUE 950
EOF

exit 0
