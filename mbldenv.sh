#!/bin/bash
# ##########################################################
# ALPS(Android2.3 based) build environment profile setting
# ##########################################################
# Overwrite JAVA_HOME environment variable setting if already exists
JAVA_HOME=/usr/lib/jvm/java-6-openjdk-i386
export JAVA_HOME

# Overwrite ANDROID_JAVA_HOME environment variable setting if already exists
ANDROID_JAVA_HOME=/usr/lib/jvm/java-6-openjdk-i386
export ANDROID_JAVA_HOME

# Overwrite PATH environment setting for JDK & arm-eabi if already exists
# PATH=/usr/lib/jvm/java-6-openjdk-i386/bin:./prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin:$PATH
# export PATH

# Add MediaTek developed Python libraries path into PYTHONPATH
if [ -z "$PYTHONPATH" ]; then
  PYTHONPATH=$PWD/mediatek/build/tools
else
  PYTHONPATH=$PWD/mediatek/build/tools:$PYTHONPATH
fi
export PYTHONPATH

export ARCH=arm
export CROSS_COMPILE=/home/zormax/android/kernel/toolchains/arm-eabi-linaro-4.6.2/bin/arm-eabi-
export PROJECT=kpt75_ics
export TARGET_PRODUCT=kpt75_ics

