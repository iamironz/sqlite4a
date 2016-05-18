package sqlite4a;

import android.content.Context;
import android.support.annotation.NonNull;

import com.getkeepsafe.relinker.ReLinker;

/**
 * @author Daniel Serdyukov
 */
@SuppressWarnings("JniMissingFunction")
public class SQLite {

    public static final int OPEN_READONLY = 0x00000001;

    public static final int OPEN_READWRITE = 0x00000002;

    public static final int OPEN_CREATE = 0x00000004;

    public static final int OPEN_URI = 0b1000000;

    public static final int SQLITE_OPEN_NOMUTEX = 0x00008000;

    public static final int SQLITE_OPEN_FULLMUTEX = 0x00010000;

    private static final int BUSY_TIMEOUT_MS = 2500;

    public static void loadLibrary(@NonNull Context context) {
        ReLinker.loadLibrary(context, "sqlite3x");
    }

    public static native String getVersion();

    public static SQLiteDb open(@NonNull String path) {
        return open(path, OPEN_READWRITE | OPEN_CREATE);
    }

    public static SQLiteDb open(@NonNull String path, int flags) {
        return open(path, flags, BUSY_TIMEOUT_MS);
    }

    public static SQLiteDb open(@NonNull String path, int flags, int busyTimeout) {
        return SQLiteDbImpl.open(path, flags, busyTimeout);
    }

}
