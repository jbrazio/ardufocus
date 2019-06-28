#!/usr/bin/env bash
cat << EOF >> ardufocus/config.h
#include "assert.h"
#endif
EOF

cat ardufocus/config.h
exit 0
