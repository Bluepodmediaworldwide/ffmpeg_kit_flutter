/*
 * Android 15+ (API 35) compatibility stub for PLATFORM_hid_write.
 *
 * The JamaisMagic ffmpeg-kit-lts-full-gpl-16kb AAR compiles libavdevice with
 * libhidapi support for HID (Bluetooth/USB input device) capture. libhidapi
 * calls PLATFORM_hid_write, an internal Android Bluetooth HID platform
 * function that was removed from the accessible symbol namespace in API 35.
 *
 * LuckyCat does NOT use FFmpeg device capture. This stub satisfies the
 * dynamic linker requirement so libavdevice.so loads cleanly.
 *
 * Why JNI_OnLoad?
 * System.loadLibrary() uses RTLD_LOCAL, which means symbols from this
 * library are NOT automatically visible to subsequently loaded libraries.
 * JNI_OnLoad calls dlopen on ourselves with RTLD_GLOBAL, which promotes
 * PLATFORM_hid_write into the namespace's global symbol group. After that,
 * when NativeLoader loads libavdevice.so, the linker finds the symbol there.
 */

#include <stddef.h>
#include <dlfcn.h>
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "hidapi_compat"

/*
 * Stub: always returns -1 (error — HID write not supported).
 * Signature matches what libhidapi expects on Android.
 */
int PLATFORM_hid_write(void *dev, const unsigned char *data, size_t length) {
    (void)dev;
    (void)data;
    (void)length;
    return -1;
}

/*
 * Promote this library to RTLD_GLOBAL so PLATFORM_hid_write is visible
 * to libavdevice.so when it is subsequently loaded by NativeLoader.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    (void)vm;
    (void)reserved;

    Dl_info info;
    if (dladdr((void *)&PLATFORM_hid_write, &info) && info.dli_fname) {
        void *handle = dlopen(info.dli_fname, RTLD_NOW | RTLD_GLOBAL);
        if (handle) {
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG,
                "Promoted to RTLD_GLOBAL: %s", info.dli_fname);
        } else {
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG,
                "dlopen RTLD_GLOBAL failed: %s", dlerror());
        }
    } else {
        __android_log_print(ANDROID_LOG_WARN, LOG_TAG,
            "dladdr failed — could not locate own library path");
    }

    return JNI_VERSION_1_6;
}
