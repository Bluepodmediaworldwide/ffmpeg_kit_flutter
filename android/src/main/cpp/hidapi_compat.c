/*
 * Android 15+ (API 35) compatibility stub for PLATFORM_hid_write.
 *
 * The JamaisMagic ffmpeg-kit-lts-full-gpl-16kb AAR compiles libavdevice with
 * libhidapi support for HID (Bluetooth/USB input device) capture. libhidapi
 * calls PLATFORM_hid_write, an internal Android Bluetooth HID platform
 * function that was removed from the public linker namespace in API 35.
 *
 * LuckyCat does NOT use FFmpeg device capture — video is picked from the
 * gallery via Flutter image_picker / photo_manager, not captured via
 * libavdevice. This stub satisfies the dynamic linker's undefined-symbol
 * requirement so libavdevice.so loads cleanly without crashing.
 *
 * The stub always returns -1 (error), which is the correct behaviour for
 * a device-capture path that will never be exercised.
 */

#include <stddef.h>

/*
 * Stub implementation: returns -1 to indicate "not supported".
 * Signature matches the HID write function expected by libhidapi on Android.
 */
int PLATFORM_hid_write(void *dev, const unsigned char *data, size_t length) {
    (void)dev;
    (void)data;
    (void)length;
    return -1;
}
