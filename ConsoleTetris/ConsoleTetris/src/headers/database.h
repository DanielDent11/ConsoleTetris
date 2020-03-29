#pragma once

#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class DataBase
{
public:
	DataBase()
	{
		try
		{
			m_driver = get_driver_instance();
			m_con = m_driver->connect("localhost", "root", "9w20dfg");
			m_con->setSchema("console_tetris");
		}
		catch (sql::SQLException &e)
		{
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line "
				<< __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		}
	}

	~DataBase()
	{
		delete m_stmt;
		delete m_con;
	}

	sql::ResultSet *ExecuteQuery(std::string query)
	{
		sql::ResultSet *res = nullptr;
		try
		{
			m_stmt = m_con->createStatement();
			res = m_stmt->executeQuery(query);
			
			delete m_stmt;
		}
		catch (sql::SQLException &e)
		{
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line "
				<< __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

			delete m_stmt;
			throw;
		}

		return res;
	}

	int ExecuteUpdate(std::string update)
	{
		int res = 0;
		try
		{
			m_stmt = m_con->createStatement();
			res = m_stmt->executeUpdate(update);

			delete m_stmt;
		}
		catch (sql::SQLException &e)
		{
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line "
				<< __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;

			delete m_stmt;
			throw;
		}

		return res;
	}

	void TestQuery()
	{
		//cout << endl;
		//cout << "Running 'SELECT * FROM `test_table` "
		//	 << endl;

		//try
		//{
		//	ExecuteQuery("SELECT * FROM `test_table`");
		//
		//	while (res->next())
		//	{
		//		cout << "\t... MySQL replies: ";
		//		/* Access column data by alias or column name */
		//		cout << m_res->getString(2) << endl;
		//	}
	
		//}
		//catch (sql::SQLException &e)
		//{
		//	cout << "# ERR: SQLException in " << __FILE__;
		//	cout << "(" << __FUNCTION__ << ") on line "
		//		<< __LINE__ << endl;
		//	cout << "# ERR: " << e.what();
		//	cout << " (MySQL error code: " << e.getErrorCode();
		//	cout << ", SQLState: " << e.getSQLState() << " )" << endl;

		//	throw;
		//}

		//cout << endl;
	}

private:
	sql::Driver *m_driver;
	sql::Connection *m_con;
	sql::Statement *m_stmt;
};