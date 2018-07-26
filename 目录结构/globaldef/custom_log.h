#ifndef CUSTOM_LOG_H_H_
#define CUSTOM_LOG_H_H_

#if !MACIOS

#include <android/log.h>

#define TAG "OITJNI"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
#else
#define LOGI printf
#define LOGW printf
#define LOGE printf
#define LOGD printf
#endif

#endif
