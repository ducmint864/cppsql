#include <iostream>
#include <stdio.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;

const string SQL_HOST                  = "localhost";
const string SQL_USER                  = "root";
const string SQL_AUTHENTICATION_STRING = "dm";
const string SQL_DATABASE              = "transactions_info";              

void Trim(std::string& word)
{
    if (word.empty()) return;

    // Trim spaces from left side
    while (word.find(" ") == 0)
    {
        word.erase(0, 1);
    }

    // Trim spaces from right side
    size_t len = word.size();
    while (word.rfind(" ") == --len)
    {
        word.erase(len, len + 1);
    }
}

class User
{
    string loginUsername;
    string loginPassword;
    string phoneNumber;

    public:
    string fullName;

    User(const string& lU, const string& lP, const string& fN, const string& pN)
    {
        this->loginUsername = lU;
        this->loginPassword = lP;
        this->fullName = fN;
        this->phoneNumber = pN;
    }

    ~User() {}
};

int main(int argc, char **arg)
{
try {
    sql::Driver* driver;
    sql::Connection* conn;
    sql::Statement* st;
    sql::PreparedStatement* pst;
    sql::ResultSet* res;
    User user("cereakiller864", "anmuticay2k4", "Nguyen Do Duc Minh", "0937802672");

    driver = get_driver_instance();
    conn = driver->connect(SQL_HOST, SQL_USER, SQL_AUTHENTICATION_STRING);
    conn->setSchema(SQL_DATABASE);

    // Code muon sach
    char option;
    string foo, query;
    bool quit = 0;
    while (!quit)
    {
        cout << "\n-------Library----------------------------------------------------------------------------" << endl;
        cout << "Library menu: (s)how all books, (b)orrow book, (q)uit" << endl;
        cout << ">>> ";
        cin >> option;
        cin.ignore();
        switch (option) {
            case 's':
            case 'S':
                pst = conn->prepareStatement("SELECT * FROM books");
                res = pst->executeQuery();            
                res->beforeFirst();
                while (res->next())
                {
                    cout << "Id: " << res->getInt("id") << ", ";
                    cout << "\"" << res->getString("ten_sach") << "\"";
                    cout << ", by " << res->getString("ten_tac_gia");
                    cout << ", " << "published in " << res->getInt("nam_xuat_ban");
                    cout << " | tinh trang: " << res->getBoolean("tinh_trang");
                    cout <<  endl; 
                } 
                delete pst;
                delete res;
                break;

            case 'q':
            case 'Q':
                cout << "Xin cam on da su dung thu vien, tam biet & hen gap lai!";
                quit = 1;
                break;

            case 'c':   
            case 'C':
                system("clear");
                break;

            case 'b':
            case 'B':
                cout << ">>> Search for books you want to borrow: ";
                getline(cin, foo);
                Trim(foo);
                query = "SELECT * FROM books WHERE ten_sach='" + foo + "'";
                // query = "SELECT * FROM books WHERE ten_sach=' subtle art of not giving a f'";
                pst = conn->prepareStatement(query);
                res = pst->executeQuery();
                if(!res->first()) { //khong tim thay sach
                    cout << "No result found for \"" << foo << "\"" << endl;
                }
                else if (res->getBoolean("tinh_trang") == 0) { //tim thay sach nhung sach da duoc muon
                    cout << "Rat tiec, quyen sach nay da duoc muon!" << endl;
                }
                else { // tim thay sach va sach chua duoc muon
                        cout << "\t*id: " << res->getInt("id") << endl;
                        cout << "\"" << res->getString("ten_sach") << "\"";
                        cout << ", by " << res->getString("ten_tac_gia");
                        cout << ", " << "published in " << res->getInt("nam_xuat_ban") << endl;
                        cout << "\tConfirm to borrow : (y)es, (n)o\n\t--> ";
                        cin >> option;
                        if (option == 'y') {
                            //update in the book table
                            query = "UPDATE books SET tinh_trang=0 WHERE ten_sach='" + foo + "'";
                            // query = "UPDATE books SET tinh_trang=0 WHERE ten_sach='The subtle art of not giving a f'";
                            st = conn->createStatement();
                            st->execute(query);
                            // update in the library table
                            query = "INSERT INTO library (nguoi_muon, ten_sach) VALUES ('" + user.fullName + "', " + "'" + foo + "')";
                            // query = "INSERT INTO library (nguoi_muon, ten_sach) VALUES ('Nguyen Do Duc Minh', 'The subtle art of not giving a f')";
                            st->execute(query);
                            cout << "Borrow successful! This book is temporarily yours" << endl;
                        }
                        else {
                            cout << "Borrow aborted!" << endl;
                        }
                } 
                delete st;
                break;

            default:
                cout << ">>> Cau lenh khong chinh xac!";
                break;
        }
    }
    

} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;
}

    cout << endl;
    return EXIT_SUCCESS;
}