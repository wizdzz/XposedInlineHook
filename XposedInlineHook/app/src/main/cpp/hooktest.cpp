#include <stdio.h>
#include <android/legacy_stdlib_inlines.h>
#include <sys/socket.h>

#include "include/inlineHook.h"
#include "hooktest.h"

#include  <android/log.h>
#include <string.h>

#define TAG "wizd_xposed"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

int (*old_rand)() = NULL;

int new_rand()
{
    return -1;
}

int hook()
{
    if (registerInlineHook((uint32_t) rand, (uint32_t) new_rand, (uint32_t **) &old_rand) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) rand) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) rand) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

int testHook() {
    int n = rand();
    LOGI("rand return: %d.", n);

    n = hook();
    LOGI("hook return: %d.", n);

    n = rand();
    LOGI("rand return: %d.", n);

    n = unHook();
    LOGI("unHook return: %d.", n);

    n = rand();
    LOGI("rand return: %d.", n);

    return 0;
}


int (*old_recvfrom)() = NULL;

//ssize_t recvfrom(int __fd, void* __buf, size_t __n, int __flags, struct sockaddr* __src_addr, socklen_t* __src_addr_length)
ssize_t new_recvfrom(int __fd, void* __buf, size_t __n, int __flags, struct sockaddr* __src_addr, socklen_t* __src_addr_length)
{
    memset(__buf, 0, __n);
    return -1;
}

void hookRecvfrom(){
    if (registerInlineHook((uint32_t) recvfrom, (uint32_t) new_recvfrom, (uint32_t **) &old_recvfrom) != ELE7EN_OK) {
        LOGD("registerInlineHook failed.");
    }
    if (inlineHook((uint32_t) recvfrom) != ELE7EN_OK) {
        LOGD("inlineHook failed.");
    }

    LOGD("hook succeed.");
}
