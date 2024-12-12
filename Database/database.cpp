#include "database.h"
#include <iostream>

using namespace std;

bool initializeDatabaseConnection(
    MYSQL &conn,
    const string &host,
    const string &user,
    const string &password,
    const string &dbName,
    unsigned int port) {
    mysql_init(&conn);
    if (!mysql_real_connect(&conn, host.c_str(), user.c_str(), password.c_str(), dbName.c_str(), port, nullptr, 0)) {
        cerr << "Database connection failed: " << mysql_error(&conn) << endl;
        return false;
    }
    return true;
}

string getPublicKey(MYSQL &conn, int loggerId) {
    string query = "SELECT pub_key FROM Loggers WHERE LID = " + to_string(loggerId);
    if (mysql_query(&conn, query.c_str())) {
        cerr << "Query failed: " << mysql_error(&conn) << endl;
        return "";
    }

    MYSQL_RES *result = mysql_store_result(&conn);
    if (!result) {
        cerr << "Result retrieval failed: " << mysql_error(&conn) << endl;
        return "";
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    string publicKey = row ? row[0] : "";
    mysql_free_result(result);
    return publicKey;
}

// pair<string, string> getLastData(MYSQL& conn) {
//     //string query = "SELECT CID, signed_hash FROM Video_data_table ORDER BY id DESC LIMIT 1";
//     //string query = "SELECT CID, signed_hash FROM Video_data_table ORDER BY created_at DESC LIMIT 1";
//     string query = "SELECT CID, signed_hash FROM Video_data_table ORDER BY CID DESC LIMIT 1";
//     if (mysql_query(&conn, query.c_str())) {
//         cerr << "Error executing query: " << mysql_error(&conn) << endl;
//         return {"", ""};
//     }

//     MYSQL_RES* result = mysql_store_result(&conn);
//     if (!result) {
//         cerr << "Error storing result: " << mysql_error(&conn) << endl;
//         return {"", ""};
//     }

//     MYSQL_ROW row = mysql_fetch_row(result);
//     string cid = row ? row[0] : "";
//     string signHash = row ? row[1] : "";

//     mysql_free_result(result);
//     return {cid, signHash};
// }

void getVerifierData(MYSQL& conn, std::string& cid, std::string& signHash, std::string& hash) {
    std::string query = "SELECT CID, signed_hash, hash FROM Video_data_table ORDER BY CID DESC LIMIT 1";

    if (mysql_query(&conn, query.c_str())) {
        std::cerr << "Error executing query: " << mysql_error(&conn) << std::endl;
        cid = "";
        signHash = "";
        hash = "";
        return;
    }

    MYSQL_RES* result = mysql_store_result(&conn);
    if (!result) {
        std::cerr << "Error storing result: " << mysql_error(&conn) << std::endl;
        cid = "";
        signHash = "";
        hash = "";
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        cid = row[0] ? row[0] : "";
        signHash = row[1] ? row[1] : "";
        hash = row[2] ? row[2] : "";
    } else {
        cid = "";
        signHash = "";
        hash = "";
    }

    mysql_free_result(result);
}

bool updateVerifyResult(MYSQL& conn, const string& cid, bool verifyResult) {
    int verifyValue = verifyResult ? 1 : 0;

    string query = "UPDATE Video_data_table SET verify_result = " + to_string(verifyValue) + 
                   " WHERE CID = '" + cid + "'";

    if (mysql_query(&conn, query.c_str())) {
        cerr << "Error executing query: " << mysql_error(&conn) << endl;
        return false;
    }

    cout << "Verification result updated successfully for CID: " << cid << endl;
    return true;
}