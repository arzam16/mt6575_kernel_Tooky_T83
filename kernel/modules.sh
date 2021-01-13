#!/bin/bash 

#make modules
rm -rf mod
mkdir mod
cp `find ./ | grep .ko$` modules.builtin mod/
cp `find ../mediatek/ | grep .ko$` modules.builtin mod/
cd mod
/home/zormax/android/kernel/toolchains/toolchain-4.6.3/bin/arm-linux-androideabi-strip --strip-debug ./*.ko