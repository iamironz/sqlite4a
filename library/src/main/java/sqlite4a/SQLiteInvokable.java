package sqlite4a;

import android.support.annotation.NonNull;

/**
 * @author Daniel Serdyukov
 */
class SQLiteInvokable {

    private final SQLiteFunc mFunc;

    SQLiteInvokable(@NonNull SQLiteFunc func) {
        mFunc = func;
    }

    public void invoke(long contextPtr, long[] valuePtrs) {
        final SQLiteValueImpl[] values = new SQLiteValueImpl[valuePtrs.length];
        for (int i = 0; i < valuePtrs.length; ++i) {
            values[i] = new SQLiteValueImpl(valuePtrs[i]);
        }
        mFunc.call(new SQLiteContextImpl(contextPtr), values);
    }

}
