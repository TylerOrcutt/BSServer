#!/bin/bash
rm -R build
mkdir build
cp -R maps/ build/
cd build
cmake ../
make
