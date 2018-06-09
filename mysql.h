#ifndef MYSQL_H
#define MYSQL_H

#include <stdlib.h>
#include <iostream>
#include <QJsonObject>
#include <QString>
#include <string>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include "login.h"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

class mySQLhandler {
	public:
		mySQLhandler();
		~mySQLhandler();
		int initDBconnection();
		void writeConsumptionTable(string &value1, float &value2, float &value3, float &value4, float &value5, float &value6, float &value7, float &value8, float &value9, float &value10);
		void getInitialValues(float &awatthr, float &bwatthr, float &cwatthr, float &avarhr, float &bvarhr, float &cvarhr, float &avahr, float &bvahr, float &cvahr);
		QJsonArray getConsumption(QString Date);
		QJsonArray getTotalConsumption();

	private:
		sql::Driver *driver;
		sql::Connection *con;
  		sql::Statement *stmt;
  		sql::PreparedStatement *pstmt;
  		sql::ResultSet *res;
};

#endif
