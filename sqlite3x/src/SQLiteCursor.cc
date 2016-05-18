#include "common.h"

extern "C" JNIEXPORT jstring JNICALL
Java_sqlite4a_SQLiteCursorImpl_nativeGetSql(JNIEnv *env, jclass caller, jlong stmtPtr) {
    return env->NewStringUTF(sqlite3_sql(reinterpret_cast<sqlite3_stmt *>(stmtPtr)));
}

extern "C" JNIEXPORT jint JNICALL
Java_sqlite4a_SQLiteCursorImpl_nativeStep(JNIEnv *env, jclass caller, jlong stmtPtr) {
    return sqlite3_step(reinterpret_cast<sqlite3_stmt *>(stmtPtr));
}

extern "C" JNIEXPORT jlong JNICALL
Java_sqlite4a_SQLiteCursorImpl_nativeGetColumnLong(JNIEnv *env, jclass caller, jlong stmtPtr, jint index) {
    return sqlite3_column_int64(reinterpret_cast<sqlite3_stmt *>(stmtPtr), index);
}

extern "C" JNIEXPORT jdouble JNICALL
Java_sqlite4a_SQLiteCursorImpl_nativeGetColumnDouble(JNIEnv *env, jclass caller, jlong stmtPtr, jint index) {
    return sqlite3_column_double(reinterpret_cast<sqlite3_stmt *>(stmtPtr), index);
}

extern "C" JNIEXPORT jstring JNICALL
Java_sqlite4a_SQLiteCursorImpl_nativeGetColumnString(JNIEnv *env, jclass caller, jlong stmtPtr, jint index) {
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt *>(stmtPtr);
    const unsigned char *text = sqlite3_column_text(stmt, index);
    return env->NewStringUTF(reinterpret_cast<const char *>(text));
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_sqlite4a_SQLiteCursorImpl_nativeGetColumnBlob(JNIEnv *env, jclass caller, jlong stmtPtr, jint index) {
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt *>(stmtPtr);
    const void *value = sqlite3_column_blob(stmt, index);
    int size = sqlite3_column_bytes(stmt, index);
    jbyteArray valueArr = env->NewByteArray(size);
    env->SetByteArrayRegion(valueArr, 0, size, reinterpret_cast<const jbyte *>(value));
    return valueArr;
}

extern "C" JNIEXPORT jint JNICALL
Java_sqlite4a_SQLiteCursorImpl_nativeGetColumnCount(JNIEnv *env, jclass caller, jlong stmtPtr) {
    return sqlite3_column_count(reinterpret_cast<sqlite3_stmt *>(stmtPtr));
}

extern "C" JNIEXPORT jstring JNICALL
Java_sqlite4a_SQLiteCursorImpl_nativeGetColumnName(JNIEnv *env, jclass caller, jlong stmtPtr, jint index) {
    return env->NewStringUTF(sqlite3_column_name(reinterpret_cast<sqlite3_stmt *>(stmtPtr), index));
}