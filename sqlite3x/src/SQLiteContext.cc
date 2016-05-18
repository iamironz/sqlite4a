#include "common.h"

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteContextImpl_nativeResultNull(JNIEnv *env, jclass type, jlong contextPtr) {
    sqlite3_result_null(reinterpret_cast<sqlite3_context *>(contextPtr));
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteContextImpl_nativeResultLong(JNIEnv *env, jclass type, jlong contextPtr, jlong result) {
    sqlite3_result_int64(reinterpret_cast<sqlite3_context *>(contextPtr), result);
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteContextImpl_nativeResultText(JNIEnv *env, jclass type, jlong contextPtr, jstring resultStr) {
    sqlite3_context *context = reinterpret_cast<sqlite3_context *>(contextPtr);
    const char *result = env->GetStringUTFChars(resultStr, nullptr);
    sqlite3_result_text(context, result, static_cast<int>(strlen(result)), SQLITE_TRANSIENT);
    env->ReleaseStringUTFChars(resultStr, result);
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteContextImpl_nativeResultDouble(JNIEnv *env, jclass type, jlong contextPtr, jdouble result) {
    sqlite3_result_double(reinterpret_cast<sqlite3_context *>(contextPtr), result);
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteContextImpl_nativeResultBlob(JNIEnv *env, jclass type, jlong contextPtr, jbyteArray resultArr) {
    sqlite3_context *context = reinterpret_cast<sqlite3_context *>(contextPtr);
    jbyte *result = env->GetByteArrayElements(resultArr, nullptr);
    sqlite3_result_blob(context, result, env->GetArrayLength(resultArr), SQLITE_TRANSIENT);
    env->ReleaseByteArrayElements(resultArr, result, 0);
}
