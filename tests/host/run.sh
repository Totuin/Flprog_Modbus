#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."
for cxx in g++ clang++; do
  for buf in 64 250; do
    exe="/tmp/$(basename "$PWD")-$(basename "$cxx")-$buf-$$"
    "$cxx" -std=c++11 -O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize-recover=all -DFLPROG_MODBUS_BUFER_SIZE="$buf" \
      -Itests/host -Isrc -Isrc/base -Isrc/rtu \
      tests/host/test_modbus.cpp src/flprogModbusUtilites.cpp src/base/flprogModbusData.cpp src/base/flprogModbus.cpp src/flprogModbusRTU.cpp \
      -o "$exe"
    "$exe"
    rm -f "$exe"
  done
done
