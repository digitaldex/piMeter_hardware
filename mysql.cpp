#include "mysql.h"

using namespace std;

mySQLhandler::mySQLhandler() {

}

int mySQLhandler::initDBconnection() {
	try {
	   	driver = get_driver_instance();
        con = driver->connect(HOST, USER, PASS);
        con->setSchema(DB);
        stmt = con->createStatement();
	    stmt->execute("CREATE TABLE IF NOT EXISTS consumption_table (id INTEGER PRIMARY KEY AUTO_INCREMENT, timestamp DATETIME, awatthr FLOAT, bwatthr FLOAT, cwatthr FLOAT, avarhr FLOAT, bvarhr FLOAT, cvarhr FLOAT, avahr FLOAT, bvahr FLOAT, cvahr FLOAT)");
	    delete stmt;
	} catch (sql::SQLException &e) {
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	return 1;
}

void mySQLhandler::writeConsumptionTable(string &value1, float &value2, float &value3, float &value4, float &value5, float &value6, float &value7, float &value8, float &value9, float &value10) {
	try {
		pstmt = con->prepareStatement("INSERT INTO consumption_table (timestamp, awatthr, bwatthr, cwatthr, avarhr, bvarhr, cvarhr, avahr, bvahr, cvahr) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
  		pstmt->setDateTime(1, value1);
        pstmt->setDouble(2, value2);
        pstmt->setDouble(3, value3);
        pstmt->setDouble(4, value4);
        pstmt->setDouble(5, value5);
        pstmt->setDouble(6, value6);
        pstmt->setDouble(7, value7);
        pstmt->setDouble(8, value8);
        pstmt->setDouble(9, value9);
        pstmt->setDouble(10, value10);
        pstmt->execute();
  		delete pstmt;
  	} catch (sql::SQLException &e) {
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}

void mySQLhandler::getInitialValues(float &awatthr, float &bwatthr, float &cwatthr, float &avarhr, float &bvarhr, float &cvarhr, float &avahr, float &bvahr, float &cvahr) {
	try {
		pstmt = con->prepareStatement("SELECT awatthr, bwatthr, cwatthr, avarhr, bvarhr, cvarhr, avahr, bvahr, cvahr FROM consumption_table WHERE id = (SELECT MAX(id) FROM consumption_table)");
  		res = pstmt->executeQuery();
  		res->afterLast();
  		res->previous();
  		awatthr = res->getDouble("awatthr");
  		bwatthr = res->getDouble("bwatthr");
  		cwatthr = res->getDouble("cwatthr");
  		avarhr = res->getDouble("avarhr");
  		bvarhr = res->getDouble("bvarhr");
  		cvarhr = res->getDouble("cvarhr");
  		avahr = res->getDouble("avahr");
  		bvahr = res->getDouble("bvahr");
  		cvahr = res->getDouble("cvahr");
  		delete res;
  		delete pstmt;
  	} catch (sql::SQLException &e) {
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}

QJsonArray mySQLhandler::getConsumption(QString Date) {
    QJsonObject object;
    QJsonArray returnData;
    int val = Date.size();
	try {
        switch(val) {
            case 10:
                pstmt = con->prepareStatement("SELECT (MAX(awatthr) - MIN(awatthr)) AS awatthr_diff, (MAX(bwatthr) - MIN(bwatthr)) AS bwatthr_diff, (MAX(cwatthr) - MIN(cwatthr)) AS cwatthr_diff, timestamp FROM `consumption_table` WHERE timestamp > '" + Date.toStdString() + " 00:00:00' AND timestamp < '" + Date.toStdString() + " 23:59:59' GROUP BY HOUR(timestamp)");
                break;
            case 7:
                pstmt = con->prepareStatement("SELECT (MAX(awatthr) - MIN(awatthr)) AS awatthr_diff, (MAX(bwatthr) - MIN(bwatthr)) AS bwatthr_diff, (MAX(cwatthr) - MIN(cwatthr)) AS cwatthr_diff, timestamp FROM `consumption_table` WHERE timestamp > '" + Date.toStdString() + "-01 00:00:00' AND timestamp < '" + Date.toStdString() + "-31 23:59:59' GROUP BY DAY(timestamp)");
                break;
            case 4:
                pstmt = con->prepareStatement("SELECT (MAX(awatthr) - MIN(awatthr)) AS awatthr_diff, (MAX(bwatthr) - MIN(bwatthr)) AS bwatthr_diff, (MAX(cwatthr) - MIN(cwatthr)) AS cwatthr_diff, timestamp FROM `consumption_table` WHERE timestamp > '" + Date.toStdString() + "01-01 00:00:00' AND timestamp < '" + Date.toStdString() + "12-31 23:59:59' GROUP BY YEAR(timestamp)");
                break;
        }
        res = pstmt->executeQuery();
  		while(res->next()) {
            object.insert(QStringLiteral("Timestamp"), QString::fromStdString(string(res->getString("Timestamp"))));
            object.insert(QStringLiteral("AWATTHR"), (float)((int)(res->getDouble("awatthr_diff")) * 100) / 100.0);
            object.insert(QStringLiteral("BWATTHR"), (float)((int)(res->getDouble("bwatthr_diff")) * 100) / 100.0);
            object.insert(QStringLiteral("CWATTHR"), (float)((int)(res->getDouble("cwatthr_diff")) * 100) / 100.0);
            returnData.push_back(object);
  		}
  		delete res;
  		delete pstmt;
  	} catch (sql::SQLException &e) {
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
  return returnData;
}

QJsonArray mySQLhandler::getTotalConsumption() {
    QJsonObject object;
    QJsonArray returnData;
    try {
        pstmt = con->prepareStatement("SELECT awatthr, bwatthr, cwatthr FROM consumption_table WHERE id = (SELECT MAX(id) FROM consumption_table)");
        res = pstmt->executeQuery();
        while(res->next()) {
          object.insert(QStringLiteral("TotalL1watt"), float(res->getDouble("awatthr")));
          object.insert(QStringLiteral("TotalL2watt"), float(res->getDouble("bwatthr")));
          object.insert(QStringLiteral("TotalL3watt"), float(res->getDouble("cwatthr")));
          returnData.push_back(object);
      }
        delete res;
        delete pstmt;
    } catch (sql::SQLException &e) {
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    return returnData;
}

mySQLhandler::~mySQLhandler() {
	driver->threadEnd();
}
