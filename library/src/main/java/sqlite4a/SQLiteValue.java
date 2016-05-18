package sqlite4a;

/**
 * @author Daniel Serdyukov
 */
public interface SQLiteValue {

    long longValue();

    double doubleValue();

    String stringValue();

    byte[] blobValue();

}
