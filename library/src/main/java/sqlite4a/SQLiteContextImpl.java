package sqlite4a;

/**
 * @author Daniel Serdyukov
 */
@SuppressWarnings("JniMissingFunction")
class SQLiteContextImpl implements SQLiteContext {

    private final long mContextPtr;

    SQLiteContextImpl(long contextPtr) {
        mContextPtr = contextPtr;
    }

    private static native void nativeResultNull(long contextPtr);

    private static native void nativeResultLong(long contextPtr, long result);

    private static native void nativeResultText(long contextPtr, String result);

    private static native void nativeResultDouble(long contextPtr, double result);

    private static native void nativeResultBlob(long contextPtr, byte[] result);

    @Override
    public void resultNull() {
        nativeResultNull(mContextPtr);
    }

    @Override
    public void resultLong(long result) {
        nativeResultLong(mContextPtr, result);
    }

    @Override
    public void resultString(String result) {
        if (result != null) {
            nativeResultText(mContextPtr, result);
        }
    }

    @Override
    public void resultDouble(double result) {
        nativeResultDouble(mContextPtr, result);
    }

    @Override
    public void resultBlob(byte[] result) {
        if (result != null) {
            nativeResultBlob(mContextPtr, result);
        }
    }

}
