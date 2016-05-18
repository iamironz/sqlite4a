package sqlite4a;

/**
 * @author Daniel Serdyukov
 */
@SuppressWarnings("JniMissingFunction")
class SQLiteValueImpl implements SQLiteValue {

    private final long mValuePtr;

    SQLiteValueImpl(long valuePtr) {
        mValuePtr = valuePtr;
    }

    static native long nativeLongValue(long valuePtr);

    static native String nativeStringValue(long valuePtr);

    static native double nativeDoubleValue(long valuePtr);

    static native byte[] nativeBlobValue(long valuePtr);

    public long longValue() {
        return nativeLongValue(mValuePtr);
    }

    public String stringValue() {
        return nativeStringValue(mValuePtr);
    }

    public double doubleValue() {
        return nativeDoubleValue(mValuePtr);
    }

    public byte[] blobValue() {
        return nativeBlobValue(mValuePtr);
    }

}
