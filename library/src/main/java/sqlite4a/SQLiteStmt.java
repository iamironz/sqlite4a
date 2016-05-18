package sqlite4a;

import android.support.annotation.NonNull;

import java.io.Closeable;

/**
 * @author Daniel Serdyukov
 */
public interface SQLiteStmt extends Closeable {

    void bindNull(int index);

    void bindLong(int index, long value);

    void bindDouble(int index, double value);

    void bindString(int index, String value);

    void bindBlob(int index, byte[] value);

    void clearBindings();

    long executeInsert();

    @NonNull
    SQLiteCursor executeSelect();

    int executeUpdateDelete();

    @Override
    void close();

}
