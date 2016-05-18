#include "common.h"

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteStmtImpl_nativeBindNull(JNIEnv *env, jclass caller, jlong stmtPtr, jint index) {
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt *>(stmtPtr);
    int ret = sqlite3_bind_null(stmt, index);
    if (SQLITE_OK != ret) {
        throw_sqlite_exception(env, ret, sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteStmtImpl_nativeBindLong(JNIEnv *env, jclass caller, jlong stmtPtr, jint index, jlong value) {
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt *>(stmtPtr);
    int ret = sqlite3_bind_int64(stmt, index, value);
    if (SQLITE_OK != ret) {
        throw_sqlite_exception(env, ret, sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteStmtImpl_nativeBindDouble(JNIEnv *env, jclass caller, jlong stmtPtr, jint index, jdouble value) {
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt *>(stmtPtr);
    int ret = sqlite3_bind_double(stmt, index, value);
    if (SQLITE_OK != ret) {
        throw_sqlite_exception(env, ret, sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteStmtImpl_nativeBindString(JNIEnv *env, jclass caller, jlong stmtPtr, jint index,
        jstring valueStr) {
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt *>(stmtPtr);
    const char *value = env->GetStringUTFChars(valueStr, nullptr);
    int ret = sqlite3_bind_text(stmt, index, value, static_cast<int>(strlen(value)), SQLITE_TRANSIENT);
    env->ReleaseStringUTFChars(valueStr, value);
    if (SQLITE_OK != ret) {
        sqlite3 *db = sqlite3_db_handle(stmt);
        throw_sqlite_exception(env, ret, sqlite3_errmsg(db));
    }
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteStmtImpl_nativeBindBlob(JNIEnv *env, jclass caller, jlong stmtPtr, jint index,
        jbyteArray valueArr) {
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt *>(stmtPtr);
    jbyte *value = env->GetByteArrayElements(valueArr, nullptr);
    int ret = sqlite3_bind_blob(stmt, index, value, env->GetArrayLength(valueArr), SQLITE_TRANSIENT);
    env->ReleaseByteArrayElements(valueArr, value, 0);
    if (SQLITE_OK != ret) {
        sqlite3 *db = sqlite3_db_handle(stmt);
        throw_sqlite_exception(env, ret, sqlite3_errmsg(db));
    }
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteStmtImpl_nativeClearBindings(JNIEnv *env, jclass caller, jlong stmtPtr) {
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt *>(stmtPtr);
    int ret = sqlite3_reset(stmt);
    if (ret == SQLITE_OK) {
        ret = sqlite3_clear_bindings(stmt);
    }
    if (SQLITE_OK != ret) {
        throw_sqlite_exception(env, ret, sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
}

extern "C" JNIEXPORT jlong JNICALL
Java_sqlite4a_SQLiteStmtImpl_nativeExecuteInsert(JNIEnv *env, jclass caller, jlong stmtPtr) {
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt *>(stmtPtr);
    sqlite3 *db = sqlite3_db_handle(stmt);
    int ret = sqlite3_step(stmt);
    if (SQLITE_DONE != ret) {
        sqlite3_finalize(stmt);
        throw_sqlite_exception(env, ret, sqlite3_errmsg(db));
        return -1;
    }
    return sqlite3_last_insert_rowid(db);
}

extern "C" JNIEXPORT jint JNICALL
Java_sqlite4a_SQLiteStmtImpl_nativeExecuteUpdateDelete(JNIEnv *env, jclass caller, jlong stmtPtr) {
    sqlite3_stmt *stmt = reinterpret_cast<sqlite3_stmt *>(stmtPtr);
    sqlite3 *db = sqlite3_db_handle(stmt);
    int ret = sqlite3_step(stmt);
    if (SQLITE_DONE != ret) {
        sqlite3_finalize(stmt);
        throw_sqlite_exception(env, ret, sqlite3_errmsg(db));
        return -1;
    }
    return sqlite3_changes(db);
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteStmtImpl_nativeFinalize(JNIEnv *env, jclass caller, jlong stmtPtr) {
    sqlite3_finalize(reinterpret_cast<sqlite3_stmt *>(stmtPtr));
}
