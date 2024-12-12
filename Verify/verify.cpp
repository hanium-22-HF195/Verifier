#include "verify.h"


// RSA 공개키 객체 생성
RSA* createPublicRSA(const std::string& key) {
    RSA* rsa = nullptr;
    BIO* keybio = BIO_new_mem_buf(key.c_str(), -1);
    if (!keybio) {
        throw std::runtime_error("Failed to create BIO for public key");
    }
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, nullptr, nullptr);
    BIO_free(keybio);
    if (!rsa) {
        throw std::runtime_error("Failed to create RSA object from public key");
    }
    return rsa;
}

// Base64 디코딩
void Base64Decode(const char* b64message, unsigned char** buffer, size_t* length) {
    BIO* bio = BIO_new_mem_buf(b64message, -1);
    BIO* b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    size_t decodeLen = strlen(b64message) * 3 / 4;
    *buffer = (unsigned char*)malloc(decodeLen + 1);
    (*buffer)[decodeLen] = '\0';

    *length = BIO_read(bio, *buffer, strlen(b64message));
    BIO_free_all(bio);
}

// RSA 서명 검증
bool RSAVerifySignature(RSA* rsa, unsigned char* MsgHash, size_t MsgHashLen, const char* Msg, size_t MsgLen, bool* Authentic) {
    *Authentic = false;
    EVP_PKEY* pubKey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pubKey, rsa);
    EVP_MD_CTX* ctx = EVP_MD_CTX_create();

    if (EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, pubKey) <= 0) {
        return false;
    }
    if (EVP_DigestVerifyUpdate(ctx, Msg, MsgLen) <= 0) {
        return false;
    }
    int authStatus = EVP_DigestVerifyFinal(ctx, MsgHash, MsgHashLen);
    if (authStatus == 1) {
        *Authentic = true;
        EVP_MD_CTX_free(ctx);
        return true;
    } else if (authStatus == 0) {
        *Authentic = false;
        EVP_MD_CTX_free(ctx);
        return true;
    } else {
        *Authentic = false;
        EVP_MD_CTX_free(ctx);
        return false;
    }
}

// 검증 함수
bool verifySignature(const std::string& publicKey, const std::string& plainText, const char* signatureBase64) {
    RSA* publicRSA = createPublicRSA(publicKey);
    unsigned char* encMessage = nullptr;
    size_t encMessageLength = 0;
    bool authentic = false;

    Base64Decode(signatureBase64, &encMessage, &encMessageLength);

    bool result = RSAVerifySignature(publicRSA, encMessage, encMessageLength, plainText.c_str(), plainText.length(), &authentic);

    free(encMessage);
    RSA_free(publicRSA);

    return result && authentic;
}
