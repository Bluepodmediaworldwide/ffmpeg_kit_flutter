/*
 * Android 15+ (API 35+) compatibility stubs for PLATFORM_hid_* symbols.
 *
 * The JamaisMagic ffmpeg-kit-lts-full-gpl-16kb AAR compiles libavdevice with
 * libhidapi support for HID (Bluetooth/USB input device) capture. libhidapi
 * calls the full PLATFORM_hid_* API — internal Android Bluetooth HID platform
 * functions that were removed from the app-accessible symbol namespace in
 * Android 15 (API 35).
 *
 * LuckyCat does NOT use FFmpeg device capture. These stubs satisfy the
 * dynamic linker so libavdevice.so loads cleanly on Android 15+.
 *
 * The patched libavdevice.so (in jniLibs/) has this library listed as
 * DT_NEEDED, so the linker loads libhidapi_compat.so before libavdevice.so
 * and resolves all PLATFORM_hid_* symbols from here.
 *
 * JNI_OnLoad also attempts RTLD_GLOBAL promotion as a belt-and-suspenders
 * measure, though DT_NEEDED is the primary mechanism.
 */

#include <stddef.h>
#include <dlfcn.h>
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "hidapi_compat"

/* ---- hidapi opaque types ------------------------------------------------- */

/* hid_device is an opaque handle; callers only ever see a pointer to it. */
typedef struct hid_device_info {
    char  *path;
    unsigned short vendor_id;
    unsigned short product_id;
    wchar_t *serial_number;
    unsigned short release_number;
    wchar_t *manufacturer_string;
    wchar_t *product_string;
    unsigned short usage_page;
    unsigned short usage;
    int interface_number;
    struct hid_device_info *next;
} hid_device_info;

/* ---- stubs: every function is a harmless no-op or returns an error ------- */

/* hid_init() / hid_exit(): 0 = success */
int PLATFORM_hid_init(void) { return 0; }
int PLATFORM_hid_exit(void) { return 0; }

/* hid_enumerate(): returns NULL (no devices) */
hid_device_info *PLATFORM_hid_enumerate(unsigned short vendor_id,
                                         unsigned short product_id) {
    (void)vendor_id; (void)product_id;
    return NULL;
}

/* hid_free_enumeration(): no-op */
void PLATFORM_hid_free_enumeration(hid_device_info *devs) { (void)devs; }

/* hid_open_path(): returns NULL (open failed) */
void *PLATFORM_hid_open_path(const char *path) { (void)path; return NULL; }

/* hid_close(): no-op */
void PLATFORM_hid_close(void *dev) { (void)dev; }

/* hid_write(): -1 = error */
int PLATFORM_hid_write(void *dev, const unsigned char *data, size_t length) {
    (void)dev; (void)data; (void)length;
    return -1;
}

/* hid_read() / hid_read_timeout(): -1 = error */
int PLATFORM_hid_read(void *dev, unsigned char *data, size_t length) {
    (void)dev; (void)data; (void)length;
    return -1;
}
int PLATFORM_hid_read_timeout(void *dev, unsigned char *data,
                               size_t length, int milliseconds) {
    (void)dev; (void)data; (void)length; (void)milliseconds;
    return -1;
}

/* hid_send_feature_report() / hid_get_feature_report(): -1 = error */
int PLATFORM_hid_send_feature_report(void *dev, const unsigned char *data,
                                      size_t length) {
    (void)dev; (void)data; (void)length;
    return -1;
}
int PLATFORM_hid_get_feature_report(void *dev, unsigned char *data,
                                     size_t length) {
    (void)dev; (void)data; (void)length;
    return -1;
}

/* hid_set_nonblocking(): -1 = error */
int PLATFORM_hid_set_nonblocking(void *dev, int nonblock) {
    (void)dev; (void)nonblock;
    return -1;
}

/* String getters: -1 = error */
int PLATFORM_hid_get_manufacturer_string(void *dev, wchar_t *string,
                                          size_t maxlen) {
    (void)dev; (void)string; (void)maxlen;
    return -1;
}
int PLATFORM_hid_get_product_string(void *dev, wchar_t *string,
                                     size_t maxlen) {
    (void)dev; (void)string; (void)maxlen;
    return -1;
}
int PLATFORM_hid_get_serial_number_string(void *dev, wchar_t *string,
                                           size_t maxlen) {
    (void)dev; (void)string; (void)maxlen;
    return -1;
}
int PLATFORM_hid_get_indexed_string(void *dev, int string_index,
                                     wchar_t *string, size_t maxlen) {
    (void)dev; (void)string_index; (void)string; (void)maxlen;
    return -1;
}

/* hid_error(): NULL = no error string */
const wchar_t *PLATFORM_hid_error(void *dev) { (void)dev; return NULL; }

/* ---- JNI_OnLoad: belt-and-suspenders RTLD_GLOBAL promotion --------------- */

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    (void)vm; (void)reserved;

    Dl_info info;
    if (dladdr((void *)&PLATFORM_hid_write, &info) && info.dli_fname) {
        void *handle = dlopen(info.dli_fname, RTLD_NOW | RTLD_GLOBAL);
        if (handle) {
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG,
                "Loaded via DT_NEEDED + RTLD_GLOBAL: %s", info.dli_fname);
        } else {
            __android_log_print(ANDROID_LOG_WARN, LOG_TAG,
                "dlopen RTLD_GLOBAL failed: %s", dlerror());
        }
    }

    return JNI_VERSION_1_6;
}
