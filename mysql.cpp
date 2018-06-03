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

QJsonArray mySQLhandler::getDayConsumption(QString Date) {
  QJsonObject object;
  QJsonArray returnData;
	try {
		pstmt = con->prepareStatement("select h.*, truncate((h.awatthr - hprev.awatthr),2) as awatthr_diff, truncate((h.bwatthr - hprev.bwatthr),2) as bwatthr_diff, truncate((h.cwatthr - hprev.cwatthr),2) as cwatthr_diff from (select extract(hour from timestamp) as theInterval, awatthr as awatthr, bwatthr as bwatthr, cwatthr as cwatthr, timestamp as Timestamp from consumption_table where date_format(timestamp, '%d-%m-%Y') = '" + Date.toStdString() + "' group by extract(hour from timestamp)) h left join (select extract(hour from timestamp) as theInterval, awatthr as awatthr, bwatthr as bwatthr, cwatthr as cwatthr from consumption_table where date_format(timestamp, '%d-%m-%Y') = '" + Date.toStdString() + "' group by extract(hour from timestamp)) hprev on h.theInterval = hprev.theInterval + 1;");
  		res = pstmt->executeQuery();
  		while(res->next()) {
          object.insert(QStringLiteral("Timestamp"), QString::fromStdString(string(res->getString("Timestamp"))));
  				object.insert(QStringLiteral("AWATTHR"), float(res->getDouble("awatthr_diff")));
  				object.insert(QStringLiteral("BWATTHR"), float(res->getDouble("bwatthr_diff")));
  				object.insert(QStringLiteral("CWATTHR"), float(res->getDouble("cwatthr_diff")));
          returnData.push_back(object);
  		}
  		delete res;
  		delete pstmt;
  	} catch (sql::SQLException &e) {
  		cout << "# ERR: " << e.what();
  		cout << " (MySQL error code: " << e.getErrorCode();
  		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
  returnData.removeLast();
  return returnData;
}

mySQLhandler::~mySQLhandler() {
	driver->threadEnd();
}
