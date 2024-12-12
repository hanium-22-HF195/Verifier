#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <string>

using namespace std;

bool initializeDatabaseConnection(
    MYSQL &conn,
    const string &host,
    const string &user,
    const string &password,
    const string &dbName,
    unsigned int port);

string getPublicKey(MYSQL &conn, int loggerId);

// pair<string, string> getLastData(MYSQL& conn);
void getVerifierData(MYSQL& conn, std::string& cid, std::string& signHash, std::string& hash);
bool updateVerifyResult(MYSQL& conn, const string& cid, bool verifyResult);

#endif // DATABASE_H
