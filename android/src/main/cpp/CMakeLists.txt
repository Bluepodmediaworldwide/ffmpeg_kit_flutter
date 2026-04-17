cmake_minimum_required(VERSION 3.10)
project(hidapi_compat C)

# Build a tiny shared library that exports PLATFORM_hid_write as a no-op.
# This satisfies the dynamic linker when libavdevice.so (from the JamaisMagic
# ffmpeg-kit-full-gpl AAR) is loaded on Android 15+ (API 35), where the real
# PLATFORM_hid_write symbol is no longer available in the platform namespace.
add_library(hidapi_compat SHARED hidapi_compat.c)
