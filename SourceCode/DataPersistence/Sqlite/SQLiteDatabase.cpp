#include "SQLiteDatabase.h"
#include "SQLiteLink.h"

SQLiteDatabase::SQLiteDatabase()
{
}

SQLiteDatabase::~SQLiteDatabase()
{
    Close();
}

bool SQLiteDatabase::Open(const std::string& databaePath)
{
    sqlite3_open(databaePath.c_str(), &m_database);
    return m_database != nullptr;
}

void SQLiteDatabase::Close()
{
    if (nullptr != m_database)
    {
        sqlite3_close(m_database);
    }
}

DisposableLink SQLiteDatabase::GetDisposableLink()
{
    auto link = std::make_shared<SQLiteLink>(this);
    return std::static_pointer_cast<IDatabaseLink>(link);
}

sqlite3* SQLiteDatabase::GetDatabase()
{
    return m_database;
}
