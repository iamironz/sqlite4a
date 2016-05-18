package sqlite4a;

/**
 * @author Daniel Serdyukov
 */
public interface SQLiteCursor {

    boolean step();

    long getColumnLong(int index);

    double getColumnDouble(int index);

    String getColumnString(int index);

    byte[] getColumnBlob(int index);

    int getColumnCount();

    String getColumnName(int index);

}
