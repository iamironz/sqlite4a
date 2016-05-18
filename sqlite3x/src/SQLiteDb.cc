#include "common.h"

static struct {
    jclass clazz;
    jmethodID compare;
} gComparator;

static struct {
    jclass clazz;
    jmethodID invoke;
} gSQLiteInvokable;

static int binary_compare(void *unused, int nKey1, const void *pKey1, int nKey2, const void *pKey2) {
    int rc, n;
    n = nKey1 < nKey2 ? nKey1 : nKey2;
    rc = memcmp(pKey1, pKey2, static_cast<size_t>(n));
    if (rc == 0) {
        rc = nKey1 - nKey2;
    }
    return rc;
}

static int java_compare(void *data, int llength, const void *lvalue, int rlength, const void *rvalue) {
    JNIEnv *env;
    if (gJavaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) == JNI_OK && data) {
        std::string lhs(static_cast<const char *>(lvalue), static_cast<size_t>(llength));
        std::string rhs(static_cast<const char *>(rvalue), static_cast<size_t>(rlength));
        jobject comparator = env->NewLocalRef(reinterpret_cast<jobject>(data));
        jstring lhsStr = env->NewStringUTF(lhs.c_str());
        jstring rhsStr = env->NewStringUTF(rhs.c_str());
        int ret = env->CallIntMethod(comparator, gComparator.compare, lhsStr, rhsStr);
        env->DeleteLocalRef(lhsStr);
        env->DeleteLocalRef(rhsStr);
        env->DeleteLocalRef(comparator);
        if (env->ExceptionCheck()) {
            LOGE("An exception was thrown by custom collation.");
            env->ExceptionClear();
        }
        return ret;
    }
    return binary_compare(data, llength, lvalue, rlength, rvalue);
}

static void java_invoke(sqlite3_context *context, int argc, sqlite3_value **argv) {
    JNIEnv *env;
    if (gJavaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) == JNI_OK) {
        jlong values[argc];
        for (int i = 0; i < argc; ++i) {
            values[i] = reinterpret_cast<jlong>(argv[i]);
        }
        void *data = sqlite3_user_data(context);
        if (data != nullptr) {
            jobject func = env->NewLocalRef(reinterpret_cast<jobject>(data));
            jlongArray valuesArr = env->NewLongArray(argc);
            env->SetLongArrayRegion(valuesArr, 0, argc, values);
            env->CallVoidMethod(func, gSQLiteInvokable.invoke, reinterpret_cast<jlong>(context), valuesArr);
            env->DeleteLocalRef(valuesArr);
            env->DeleteLocalRef(func);
            if (env->ExceptionCheck()) {
                LOGE("An exception was thrown by custom function.");
                env->ExceptionClear();
            }
        }
    }
}

static void java_finalize(void *data) {
    JNIEnv *env;
    if (gJavaVm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) == JNI_OK && data) {
        env->DeleteGlobalRef(reinterpret_cast<jobject>(data));
    }
}

extern "C" JNIEXPORT jlong JNICALL
Java_sqlite4a_SQLiteDbImpl_nativeOpen(JNIEnv *env, jclass caller, jstring pathStr, jint flags, jint busyTimeout) {
    sqlite3 *db;
    const char *path = env->GetStringUTFChars(pathStr, nullptr);
    int ret = sqlite3_open_v2(path, &db, flags, nullptr);
    env->ReleaseStringUTFChars(pathStr, path);
    if (ret != SQLITE_OK) {
        throw_sqlite_exception(env, ret, sqlite3_errmsg(db));
        return 0;
    }
    if ((flags & SQLITE_OPEN_READWRITE) && sqlite3_db_readonly(db, nullptr)) {
        sqlite3_close_v2(db);
        throw_sqlite_exception(env, ret, "Could not open the database in read/write mode.");
        return 0;
    }
    ret = sqlite3_busy_timeout(db, busyTimeout);
    if (ret != SQLITE_OK) {
        throw_sqlite_exception(env, ret, "Could not set busy timeout");
        sqlite3_close(db);
        return 0;
    }
    return reinterpret_cast<jlong>(db);
}

extern "C" JNIEXPORT jint JNICALL
Java_sqlite4a_SQLiteDbImpl_nativeIsReadOnly(JNIEnv *env, jclass caller, jlong dbPtr) {
    return sqlite3_db_readonly(reinterpret_cast<sqlite3 *>(dbPtr), nullptr);
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteDbImpl_nativeEnableTracing(JNIEnv *env, jclass caller, jlong dbPtr) {
    sqlite3_trace(reinterpret_cast<sqlite3 *>(dbPtr), [](void *, const char *sql) { LOGI("%s", sql); }, nullptr);
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteDbImpl_nativeCreateCollation(JNIEnv *env, jclass caller, jlong dbPtr, jstring nameStr,
        jobject comparator) {
    sqlite3 *db = reinterpret_cast<sqlite3 *>(dbPtr);
    jobject comparatorRef = env->NewGlobalRef(comparator);
    const char *name = env->GetStringUTFChars(nameStr, nullptr);
    int ret = sqlite3_create_collation_v2(db, name, SQLITE_UTF8, reinterpret_cast<void *>(comparatorRef),
                                          &java_compare, &java_finalize);
    env->ReleaseStringUTFChars(nameStr, name);
    if (SQLITE_OK != ret) {
        throw_sqlite_exception(env, ret, sqlite3_errmsg(db));
    }
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteDbImpl_nativeCreateFunction(JNIEnv *env, jclass caller, jlong dbPtr, jstring nameStr,
        jint numArgs, jobject func) {
    sqlite3 *db = reinterpret_cast<sqlite3 *>(dbPtr);
    jobject funcRef = env->NewGlobalRef(func);
    const char *name = env->GetStringUTFChars(nameStr, nullptr);
    int ret = sqlite3_create_function_v2(db, name, numArgs, SQLITE_UTF8, reinterpret_cast<void *>(funcRef),
                                         &java_invoke, nullptr, nullptr, &java_finalize);
    env->ReleaseStringUTFChars(nameStr, name);
    if (SQLITE_OK != ret) {
        throw_sqlite_exception(env, ret, sqlite3_errmsg(db));
    }
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteDbImpl_nativeSetUserVersion(JNIEnv *env, jclass caller, jlong dbPtr, jstring versionStr) {
    sqlite3 *db = reinterpret_cast<sqlite3 *>(dbPtr);
    std::string sql("PRAGMA user_version = ");
    const char *version = env->GetStringUTFChars(versionStr, nullptr);
    sql.append(version);
    env->ReleaseStringUTFChars(versionStr, version);
    int ret = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    if (SQLITE_OK != ret) {
        throw_sqlite_exception(env, ret, sql.c_str(), sqlite3_errmsg(db));
    }
}

extern "C" JNIEXPORT jint JNICALL
Java_sqlite4a_SQLiteDbImpl_nativeGetUserVersion(JNIEnv *env, jclass caller, jlong dbPtr) {
    sqlite3 *db = reinterpret_cast<sqlite3 *>(dbPtr);
    sqlite3_stmt *stmt;
    const char *sql = "PRAGMA user_version;";
    int ret = sqlite3_prepare_v2(db, sql, static_cast<int>(strlen(sql)), &stmt, nullptr);
    if (SQLITE_OK != ret) {
        throw_sqlite_exception(env, ret, sql, sqlite3_errmsg(db));
    }
    int version = 0;
    ret = sqlite3_step(stmt);
    if (SQLITE_ROW == ret) {
        version = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return version;
}

extern "C" JNIEXPORT jint JNICALL
Java_sqlite4a_SQLiteDbImpl_nativeIsAutocommit(JNIEnv *env, jclass caller, jlong dbPtr) {
    return sqlite3_get_autocommit(reinterpret_cast<sqlite3 *>(dbPtr));
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteDbImpl_nativeExec(JNIEnv *env, jclass caller, jlong dbPtr, jstring sqlSrt) {
    sqlite3 *db = reinterpret_cast<sqlite3 *>(dbPtr);
    const char *sqlChars = env->GetStringUTFChars(sqlSrt, nullptr);
    std::string sql(sqlChars);
    env->ReleaseStringUTFChars(sqlSrt, sqlChars);
    int ret = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    if (SQLITE_OK != ret) {
        throw_sqlite_exception(env, ret, sql.c_str(), sqlite3_errmsg(db));
    }
}

extern "C" JNIEXPORT jlong JNICALL
Java_sqlite4a_SQLiteDbImpl_nativePrepare(JNIEnv *env, jclass caller, jlong dbPtr, jstring sqlSrt) {
    sqlite3 *db = reinterpret_cast<sqlite3 *>(dbPtr);
    const char *sqlChars = env->GetStringUTFChars(sqlSrt, nullptr);
    std::string sql(sqlChars);
    env->ReleaseStringUTFChars(sqlSrt, sqlChars);
    sqlite3_stmt *stmt;
    int ret = sqlite3_prepare_v2(db, sql.c_str(), static_cast<int>(sql.length()), &stmt, nullptr);
    if (SQLITE_OK != ret) {
        throw_sqlite_exception(env, ret, sql.c_str(), sqlite3_errmsg(db));
    }
    return reinterpret_cast<jlong>(stmt);
}

extern "C" JNIEXPORT void JNICALL
Java_sqlite4a_SQLiteDbImpl_nativeClose(JNIEnv *env, jclass caller, jlong dbPtr) {
    sqlite3_close_v2(reinterpret_cast<sqlite3 *>(dbPtr));
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    gJavaVm = vm;
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    gSQLiteException.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("sqlite4a/SQLiteException")));
    gComparator.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("java/util/Comparator")));
    gComparator.compare = env->GetMethodID(gComparator.clazz, "compare", "(Ljava/lang/Object;Ljava/lang/Object;)I");
    gSQLiteInvokable.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("sqlite4a/SQLiteInvokable")));
    gSQLiteInvokable.invoke = env->GetMethodID(gSQLiteInvokable.clazz, "invoke", "(J[J)V");
    sqlite3_soft_heap_limit64(SOFT_HEAP_LIMIT);
    sqlite3_initialize();
    return JNI_VERSION_1_6;
}