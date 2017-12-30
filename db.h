
using namespace std;

int createDB(const char *dbPath, sqlite3 *dbPointer, sqlite3_stmt *stmt, const char *sql) {
	int rc;
	rc = sqlite3_open(dbPath, &dbPointer);
	if(rc) {
		cerr << "SQL ERROR Opening Database: " << sqlite3_errmsg(dbPointer) << endl;
		sqlite3_close(dbPointer);
		return(1);
	}
	rc = sqlite3_prepare_v2(dbPointer, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cerr << "SQL ERROR Prepare Statement: " << sqlite3_errmsg(dbPointer) << endl;
    		return(1);
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		cerr << "SQL ERROR Step Statement: " << sqlite3_errmsg(dbPointer) << endl;
		return(1);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(dbPointer);
    return 0;
}

int writeDB(const char *dbPath, sqlite3 *dbPointer, sqlite3_stmt *stmt, const char *sql) {
	int rc;
	rc = sqlite3_open(dbPath, &dbPointer);
	if(rc) {
		cerr << "SQL ERROR Opening Database: " << sqlite3_errmsg(dbPointer) << endl;
		sqlite3_close(dbPointer);
		return(1);
	}
	rc = sqlite3_prepare_v2(dbPointer, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		cerr << "SQL ERROR Prepare Statement: " << sqlite3_errmsg(dbPointer) << endl;
    		return(1);
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		cerr << "SQL ERROR Step Statement: " << sqlite3_errmsg(dbPointer) << endl;
		return(1);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(dbPointer);
    return 0;
}
