#include <iostream>
#include "ComboEntry.h"

void ComboEntry::setUsername(std::string user)
{
    username = user;
}

void ComboEntry::setPassword(std::string pass)
{
    password = pass;
}

void ComboEntry::setDomain(std::string site)
{
    domain = site;
}

std::string ComboEntry::getUsername()
{
    return username;
}

std::string ComboEntry::getPassword()
{
    return password;
}

std::string ComboEntry::getDomain()
{
    return domain;
}
