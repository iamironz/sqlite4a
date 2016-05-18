#include "common.h"

extern "C" JNIEXPORT jstring JNICALL
Java_sqlite4a_SQLite_getVersion(JNIEnv *env, jclass caller) {
    return env->NewStringUTF(sqlite3_libversion());
}