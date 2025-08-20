#!/bin/sh
set -e -x

cd ../build
make clean
rm -rf fdrc.exe
make -j8

cp ../README.md README.txt
cp release/fdrc.exe .
/c/Program\ Files/7-Zip/7z.exe a fdrc.zip fdrc.exe README.txt
cp fdrc.zip ..
rm README.txt fdrc.exe
