#include <string>
#include <vector>
#include "ComboEntry.h"
class DecryptionHandler
{
    public:
	    std::string getMasterKey();
        std::vector<ComboEntry> decryptPasswords(std::string key, std::vector<ComboEntry> entries);
};
