#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <iostream>
#include "Database/database.h"
#include "Hash/hash.h"
#include "Verify/verify.h"

using namespace std;

bool fileExists(const string& path) {
    ifstream file(path);
    return file.good();
}

int main() {

    MYSQL conn;
    if (!initializeDatabaseConnection(conn, "192.168.0.20", "hanium", "1234", "hanium", 3306)) {
        return 1;
    }

    //1. puk key 가지고오기
    int loggerId = 1;
    string publicKey = getPublicKey(conn, loggerId);
    if (publicKey.empty()) {
        cerr << "Failed to retrieve public key." << endl;
        return 1;
    }
    cout << "Public Key: " << publicKey << endl;

    //2. 필요 정보값 받기
    string cid, signHash_DB, hash_DB;
    getVerifierData(conn, cid, signHash_DB, hash_DB);

    if (cid.empty() || signHash_DB.empty() || hash_DB.empty()) {
        cerr << "Failed to retrieve CID, signed hash, or hash." << endl;
        return 1;
    }

    cout << "--------------------get CID, hash, sign_hash-------" << endl;
    cout << "CID: " << cid << endl;
    cout << "SignHash_DB: " << signHash_DB << endl;
    cout << "Hash_DB: " << hash_DB << endl;
    cout << "---------------------------------------------------" << endl;


    string imagePath = "../VMS_Server/uploads/" + cid + ".png";
    if (!fileExists(imagePath)) {
        cerr << "Error: Image file does not exist at " << imagePath << endl;
        return -1;
    }

    //3. hash값 추출
    cv::Mat edge_result;

    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    cv::Mat image_gray;
    cv::cvtColor(image, image_gray, cv::COLOR_BGR2GRAY);

    cout << "Image size: " << image.cols << "x" << image.rows << endl;
    cv::Canny(image_gray, edge_result, 20, 40);

    string hash_result;
    make_hash(edge_result, hash_result);

    // cout << "make hash : " << hash_result << endl;

    //4. 검증 수행
    // /*  wronghash Insert Verification Results   */
    // string wronghash = "a9940344be08a0d3a3cc4b234a78962e723b2d1365066bd0377f9c53ce8f202a";
    // cout << "wronghash : " << wronghash << endl;
    // bool isVerified = verifySignature(publicKey, wronghash, signHash_DB.c_str());

    bool isVerified = verifySignature(publicKey, hash_result, signHash_DB.c_str());
    cout << "isVerified : " << isVerified << endl;

    //5. 검증 결과 updata
    if (!updateVerifyResult(conn, cid, isVerified)) {
        cerr << "Failed to update verification result." << endl;
        mysql_close(&conn);
        return 1;
    }

    edge_result.release();
    image_gray.release();
    mysql_close(&conn);
    return 0;
}
