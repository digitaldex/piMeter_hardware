
using namespace std;

int createDB(const char *sql) {
    const char *dbPath = "/home/pi/db.sqlite";
    sqlite3 *dbPointer;
    sqlite3_stmt *stmt;
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

void createDBTables() {
    /* CREATE CURRENT TABLE IF NOT EXISTS */
    const char *sql;
    sql = "CREATE TABLE if not exists Current (ID INTEGER PRIMARY KEY, Timestamp TEXT, AIRMS real, BIRMS real, CIRMS real, ISUMRMS real, AI_PCF real, BI_PCF real, CI_PCF real)";
    if(createDB(sql)) {
        cerr << "SQL ERROR: Could not create Current Database" << endl;
    };
    
    /* CREATE VOLTAGE TABLE IF NOT EXISTS */
    sql = "CREATE TABLE if not exists Voltage (ID INTEGER PRIMARY KEY, Timestamp TEXT, AVRMS real, BVRMS real, CVRMS real, AV_PCF real, BV_PCF real, CV_PCF real)";
    if(createDB(sql)) {
        cerr << "SQL ERROR: Could not create Voltage Database" << endl;
    };
    
    /* CREATE POWER TABLE IF NOT EXISTS */
    sql = "CREATE TABLE if not exists Power (ID INTEGER PRIMARY KEY, Timestamp TEXT, AWATT real, AVA real, BWATT real, BVA real, CWATT real, CVA real, PWATT_ACC real, PVAR_ACC real)";
    if(createDB(sql)) {
        cerr << "SQL ERROR: Could not create Power Database" << endl;
    };
}

int writeDB(const char *sql) {
    const char *dbPath = "/home/pi/db.sqlite";
    sqlite3 *dbPointer;
    sqlite3_stmt *stmt;
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
