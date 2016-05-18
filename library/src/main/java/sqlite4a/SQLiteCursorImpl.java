package sqlite4a;

/**
 * @author Daniel Serdyukov
 */
@SuppressWarnings("JniMissingFunction")
class SQLiteCursorImpl implements SQLiteCursor {

    private static final int SQLITE_ROW = 100;

    private final long mStmtPtr;

    SQLiteCursorImpl(long stmtPtr) {
        mStmtPtr = stmtPtr;
    }

    private static native int nativeStep(long stmtPtr);

    private static native long nativeGetColumnLong(long stmtPtr, int index);

    private static native double nativeGetColumnDouble(long stmtPtr, int index);

    private static native String nativeGetColumnString(long stmtPtr, int index);

    private static native byte[] nativeGetColumnBlob(long stmtPtr, int index);

    private static native int nativeGetColumnCount(long stmtPtr);

    private static native String nativeGetColumnName(long stmtPtr, int index);

    @Override
    public boolean step() {
        return nativeStep(mStmtPtr) == SQLITE_ROW;
    }

    @Override
    public long getColumnLong(int index) {
        return nativeGetColumnLong(mStmtPtr, index);
    }

    @Override
    public double getColumnDouble(int index) {
        return nativeGetColumnDouble(mStmtPtr, index);
    }

    @Override
    public String getColumnString(int index) {
        return nativeGetColumnString(mStmtPtr, index);
    }

    @Override
    public byte[] getColumnBlob(int index) {
        return nativeGetColumnBlob(mStmtPtr, index);
    }

    @Override
    public int getColumnCount() {
        return nativeGetColumnCount(mStmtPtr);
    }

    @Override
    public String getColumnName(int index) {
        return nativeGetColumnName(mStmtPtr, index);
    }

}
