#ifndef _COMBOENTRY_
#define _COMBOENTRY_

#include <string>

class ComboEntry
{
    std::string username, password, domain;
    public:
    	void setDomain(std::string domain);
        void setUsername(std::string user);
		void setPassword(std::string pass);
	std::string getUsername();
	std::string getPassword();	
	std::string getDomain();
};

#endif