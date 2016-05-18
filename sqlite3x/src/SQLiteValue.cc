#include "common.h"

extern "C" JNIEXPORT jlong JNICALL
Java_sqlite4a_SQLiteValueImpl_nativeLongValue(JNIEnv *env, jclass type, jlong valuePtr) {
    sqlite3_value *value = reinterpret_cast<sqlite3_value *>(valuePtr);
    return sqlite3_value_int64(value);
}

extern "C" JNIEXPORT jstring JNICALL
Java_sqlite4a_SQLiteValueImpl_nativeStringValue(JNIEnv *env, jclass type, jlong valuePtr) {
    sqlite3_value *value = reinterpret_cast<sqlite3_value *>(valuePtr);
    const unsigned char *text = sqlite3_value_text(value);
    return env->NewStringUTF(reinterpret_cast<const char *>(text));
}

extern "C" JNIEXPORT jdouble JNICALL
Java_sqlite4a_SQLiteValueImpl_nativeDoubleValue(JNIEnv *env, jclass type, jlong valuePtr) {
    sqlite3_value *value = reinterpret_cast<sqlite3_value *>(valuePtr);
    return sqlite3_value_double(value);
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_sqlite4a_SQLiteValueImpl_nativeBlobValue(JNIEnv *env, jclass type, jlong valuePtr) {
    sqlite3_value *value = reinterpret_cast<sqlite3_value *>(valuePtr);
    const void *blob = sqlite3_value_blob(value);
    int size = sqlite3_value_bytes(value);
    jbyteArray blobArr = env->NewByteArray(size);
    env->SetByteArrayRegion(blobArr, 0, size, reinterpret_cast<const jbyte *>(blob));
    return blobArr;
}
