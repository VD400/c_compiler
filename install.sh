#!/bin/bash

echo "======================================"
echo " Installing Mini-VD Compiler..."
echo "======================================"

cd /tmp
rm -rf minivd_temp_install

echo "[1/4] Fetching source code..."
git clone -q https://github.com/VD400/c_compiler.git minivd_temp_install
cd minivd_temp_install

echo "[2/4] Building compiler core..."
make compiler > /dev/null 2>&1

echo "[3/4] Installing to system path (requires sudo)..."
sudo make install

echo "[4/4] Cleaning up..."
cd /tmp
rm -rf minivd_temp_install

echo "======================================"
echo " Success! Mini-VD is ready to use."
echo " Try typing: minivd your_file.c"
echo "======================================"
