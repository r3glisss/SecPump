#!/usr/bin/bash
st-flash  --debug --reset --format binary --flash=512k write SecPump-Vuln.bin 0x8000000
