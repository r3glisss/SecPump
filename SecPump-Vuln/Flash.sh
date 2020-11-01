#!/usr/bin/bash
st-flash  --debug --reset --format binary --flash=512k write build/SecPump.bin 0x8000000
