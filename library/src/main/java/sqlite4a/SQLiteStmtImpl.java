package sqlite4a;

import android.support.annotation.NonNull;

/**
 * @author Daniel Serdyukov
 */
@SuppressWarnings("JniMissingFunction")
class SQLiteStmtImpl implements SQLiteStmt {

    private final long mStmtPtr;

    SQLiteStmtImpl(long stmtPtr) {
        mStmtPtr = stmtPtr;
    }

    private static native void nativeBindNull(long stmtPtr, int index);

    private static native void nativeBindLong(long stmtPtr, int index, long value);

    private static native void nativeBindDouble(long stmtPtr, int index, double value);

    private static native void nativeBindString(long stmtPtr, int index, String value);

    private static native void nativeBindBlob(long stmtPtr, int index, byte[] value);

    private static native void nativeClearBindings(long stmtPtr);

    private static native long nativeExecuteInsert(long stmtPtr);

    private static native int nativeExecuteUpdateDelete(long stmtPtr);

    private static native void nativeFinalize(long stmtPtr);

    @Override
    public void bindNull(int index) {
        nativeBindNull(mStmtPtr, index);
    }

    @Override
    public void bindLong(int index, long value) {
        nativeBindLong(mStmtPtr, index, value);
    }

    @Override
    public void bindDouble(int index, double value) {
        nativeBindDouble(mStmtPtr, index, value);
    }

    @Override
    public void bindString(int index, String value) {
        nativeBindString(mStmtPtr, index, value);
    }

    @Override
    public void bindBlob(int index, byte[] value) {
        nativeBindBlob(mStmtPtr, index, value);
    }

    @Override
    public void clearBindings() {
        nativeClearBindings(mStmtPtr);
    }

    @Override
    public long executeInsert() {
        return nativeExecuteInsert(mStmtPtr);
    }

    @NonNull
    @Override
    public SQLiteCursor executeSelect() {
        return new SQLiteCursorImpl(mStmtPtr);
    }

    @Override
    public int executeUpdateDelete() {
        return nativeExecuteUpdateDelete(mStmtPtr);
    }

    @Override
    public void close() {
        nativeFinalize(mStmtPtr);
    }

}
