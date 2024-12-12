#ifndef VERIFY_H
#define VERIFY_H

#include <string>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/ssl.h>
#include <cstring>
#include <stdexcept>

using namespace std;

RSA* createPublicRSA(const std::string& key);
void Base64Decode(const char* b64message, unsigned char** buffer, size_t* length);
bool RSAVerifySignature(RSA* rsa, unsigned char* MsgHash, size_t MsgHashLen, const char* Msg, size_t MsgLen, bool* Authentic);
bool verifySignature(const std::string& publicKey, const std::string& plainText, const char* signatureBase64);

#endif // VERIFY_H
