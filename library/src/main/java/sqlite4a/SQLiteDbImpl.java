package sqlite4a;

import android.support.annotation.NonNull;

import java.util.Comparator;

/**
 * @author Daniel Serdyukov
 */
@SuppressWarnings("JniMissingFunction")
class SQLiteDbImpl implements SQLiteDb {

    private final long mDbPtr;

    SQLiteDbImpl(long dbPtr) {
        mDbPtr = dbPtr;
    }

    @NonNull
    static SQLiteDbImpl open(@NonNull String path, int flags, int busyTimeout) {
        return new SQLiteDbImpl(nativeOpen(path, flags, busyTimeout));
    }

    private static native long nativeOpen(String path, int flags, int busyTimeout);

    private static native int nativeIsReadOnly(long dbPtr);

    private static native void nativeEnableTracing(long dbPtr);

    private static native void nativeCreateCollation(long dbPtr, String name, Comparator<String> comparator);

    private static native void nativeCreateFunction(long dbPtr, String name, int numArgs, SQLiteInvokable func);

    private static native void nativeSetUserVersion(long dbPtr, String version);

    private static native int nativeGetUserVersion(long dbPtr);

    private static native int nativeIsAutocommit(long dbPtr);

    private static native void nativeExec(long dbPtr, String sql);

    private static native long nativePrepare(long dbPtr, String sql);

    private static native void nativeClose(long dbPtr);

    @Override
    public boolean isReadOnly() {
        return nativeIsReadOnly(mDbPtr) > 0;
    }

    @Override
    public void enableTracing() {
        nativeEnableTracing(mDbPtr);
    }

    @Override
    public void createCollation(@NonNull String name, @NonNull Comparator<String> comparator) {
        nativeCreateCollation(mDbPtr, name, comparator);
    }

    @Override
    public void createFunction(@NonNull String name, int numArgs, @NonNull SQLiteFunc func) {
        nativeCreateFunction(mDbPtr, name, numArgs, new SQLiteInvokable(func));
    }

    @Override
    public void setUserVersion(int version) {
        nativeSetUserVersion(mDbPtr, String.valueOf(version));
    }

    @Override
    public int getUserVersion() {
        return nativeGetUserVersion(mDbPtr);
    }

    @Override
    public void begin() {
        nativeExec(mDbPtr, "BEGIN;");
    }

    @Override
    public void commit() {
        nativeExec(mDbPtr, "COMMIT;");
    }

    @Override
    public void rollback() {
        nativeExec(mDbPtr, "ROLLBACK;");
    }

    @Override
    public boolean inTransaction() {
        return nativeIsAutocommit(mDbPtr) <= 0;
    }

    @Override
    public void exec(String sql) {
        nativeExec(mDbPtr, sql);
    }

    @NonNull
    @Override
    public SQLiteStmt prepare(@NonNull String sql) {
        return new SQLiteStmtImpl(nativePrepare(mDbPtr, sql));
    }

    @Override
    public void close() {
        nativeClose(mDbPtr);
    }

}
