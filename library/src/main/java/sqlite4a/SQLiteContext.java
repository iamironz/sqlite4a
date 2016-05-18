package sqlite4a;

/**
 * @author Daniel Serdyukov
 */
public interface SQLiteContext {

    void resultNull();

    void resultLong(long result);

    void resultString(String result);

    void resultDouble(double result);

    void resultBlob(byte[] result);

}
