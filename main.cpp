#include <iostream>
#include <sstream>
#include <memory>
#include <stdexcept>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#define HOST                    "localhost"
#define USER                    "root"
#define AUTHENTICATION_STRING   "dm"
#define DATABASE                "transactions_info"

int main(int argc, const char **argv)
{
	const std::string url      =     HOST;
	const std::string user     =     USER;
	const std::string pass     =     AUTHENTICATION_STRING;
	const std::string database =     DATABASE;

	sql::Driver* driver =       get_driver_instance();
	std::unique_ptr<sql::Connection> con(driver->connect(HOST, USER, AUTHENTICATION_STRING));
	con->setSchema(database);

    return 0;    
}
