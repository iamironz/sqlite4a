package sqlite4a;

import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;

import org.hamcrest.core.Is;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;

/**
 * @author Daniel Serdyukov
 */
@RunWith(AndroidJUnit4.class)
public class SQLiteTest {

    static {
        SQLite.loadLibrary(InstrumentationRegistry.getContext());
    }

    @Test
    public void testSQLiteVersion() throws Exception {
        Assert.assertThat(SQLite.getVersion(), Is.is("3.12.2"));
    }

    @Test
    public void testOpenReadOnly() throws Exception {
        final SQLiteDb db = SQLite.open(":memory:", SQLite.OPEN_READONLY);
        Assert.assertThat(db.isReadOnly(), Is.is(true));
        db.close();
    }

    @Test
    public void testOpenReadWrite() throws Exception {
        final SQLiteDb db = SQLite.open(":memory:");
        Assert.assertThat(db.isReadOnly(), Is.is(false));
        db.close();
    }

    @Test(expected = SQLiteException.class)
    public void testOpenNonExistingPath() throws Exception {
        SQLite.open("/not/exists");
    }

}
