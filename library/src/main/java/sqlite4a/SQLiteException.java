package sqlite4a;

/**
 * @author Daniel Serdyukov
 */
public class SQLiteException extends RuntimeException {

    public SQLiteException() {
    }

    public SQLiteException(String detailMessage) {
        super(detailMessage);
    }

    public SQLiteException(String detailMessage, Throwable throwable) {
        super(detailMessage, throwable);
    }

    public SQLiteException(Throwable throwable) {
        super(throwable);
    }

}
