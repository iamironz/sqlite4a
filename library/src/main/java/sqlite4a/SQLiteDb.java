package sqlite4a;

import android.support.annotation.NonNull;

import java.io.Closeable;
import java.util.Comparator;

/**
 * @author Daniel Serdyukov
 */
public interface SQLiteDb extends Closeable {

    boolean isReadOnly();

    void enableTracing();

    void createCollation(@NonNull String name, @NonNull Comparator<String> comparator);

    void createFunction(@NonNull String name, int numArgs, @NonNull SQLiteFunc func);

    void setUserVersion(int version);

    int getUserVersion();

    void begin();

    void commit();

    void rollback();

    boolean inTransaction();

    void exec(String sql);

    @NonNull
    SQLiteStmt prepare(@NonNull String sql);

    @Override
    void close();

}
