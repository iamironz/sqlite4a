#ifndef SQLITE3X_COMMON_H
#define SQLITE3X_COMMON_H

#include <jni.h>
#include <string>
#include <android/log.h>
#include <sqlite3.h>

#ifndef LOG_TAG
#define LOG_TAG "sqlite3x"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif // LOG_TAG

static const int SOFT_HEAP_LIMIT = 8 * 1024 * 1024;

static JavaVM *gJavaVm = 0;

static struct {
    jclass clazz;
} gSQLiteException;

static void throw_sqlite_exception(JNIEnv *env, int code, const char *message) {
    if (gSQLiteException.clazz) {
        env->ThrowNew(gSQLiteException.clazz, message);
    } else {
        LOGE("%s [%d]", message, code);
    }
}

static void throw_sqlite_exception(JNIEnv *env, int code, const char *sql, const char *error) {
    std::string message(error);
    message += ", while executing: ";
    message += sql;
    if (gSQLiteException.clazz) {
        env->ThrowNew(gSQLiteException.clazz, message.c_str());
    } else {
        LOGE("%s [%d]", message.c_str(), code);
    }
}

static void throw_sqlite_exception(int code, const char *message) {
    JNIEnv *env;
    if (gJavaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) == JNI_OK) {
        throw_sqlite_exception(env, code, message);
    } else {
        LOGE("[%d] %s", code, message);
    }
}

#endif //SQLITE3X_COMMON_H
