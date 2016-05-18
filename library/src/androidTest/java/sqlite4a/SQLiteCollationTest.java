package sqlite4a;

import android.support.test.InstrumentationRegistry;
import android.support.test.runner.AndroidJUnit4;

import org.hamcrest.core.Is;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

/**
 * @author Daniel Serdyukov
 */
@RunWith(AndroidJUnit4.class)
public class SQLiteCollationTest {

    static {
        SQLite.loadLibrary(InstrumentationRegistry.getContext());
    }

    private SQLiteDb mDb;

    private List<String> mCities;

    @Before
    public void setUp() throws Exception {
        mDb = SQLite.open(":memory:");
        mDb.enableTracing();
        mDb.createCollation("NOCASE", new Comparator<String>() {
            @Override
            public int compare(String lhs, String rhs) {
                return lhs.compareToIgnoreCase(rhs);
            }
        });

        mDb.exec("CREATE TABLE cities(id INTEGER PRIMARY KEY ON CONFLICT REPLACE, name TEXT COLLATE NOCASE);");
        mCities = Arrays.asList(
                "Москва", "Санкт-Петербург", "Новосибирск", "Екатеринбург", "Нижний Новгород",
                "Казань", "Челябинск", "Омск", "Самара", "Ростов-на-Дону",
                "Los Angeles", "Chicago", "Houston", "Philadelphia", "Phoenix",
                "San Antonio", "San Diego", "Dallas", "San Jose", "Austin");
        mDb.begin();
        SQLiteStmt stmt = mDb.prepare("INSERT INTO cities VALUES(NULL, ?);");
        for (final String city : mCities) {
            stmt.clearBindings();
            stmt.bindString(1, city);
            stmt.executeInsert();
        }
        stmt.close();
        mDb.commit();
    }

    @Test
    public void testCollateNonAscii() throws Exception {
        final SQLiteStmt stmt = mDb.prepare("SELECT name FROM cities WHERE name = ?;");
        for (final String city : mCities) {
            stmt.bindString(1, city.toLowerCase());
            final SQLiteCursor cursor = stmt.executeSelect();
            Assert.assertThat(cursor.step(), Is.is(true));
            Assert.assertThat(cursor.getColumnString(0), Is.is(city));
            stmt.clearBindings();
        }
        stmt.close();
    }

    @After
    public void tearDown() throws Exception {
        mDb.close();
    }

}
