#include "SQLiteLink.h"

SQLiteLink::SQLiteLink(SQLiteDatabase* database)
    : m_database(database)
{
}

SQLiteLink::~SQLiteLink()
{
    sqlite3_finalize(m_statement);
}

bool SQLiteLink::Exec(const std::string& stmt)
{
    auto ret = sqlite3_exec(m_database->GetDatabase(), stmt.c_str(), nullptr, nullptr, nullptr);
    return ret == SQLITE_OK;
}

bool SQLiteLink::PrepareStmt(const std::string& stmt)
{
    auto ret = sqlite3_prepare_v2(m_database->GetDatabase(), stmt.c_str(), -1, &m_statement, nullptr);
    return ret == SQLITE_OK;
}

bool SQLiteLink::Evaluate()
{
    auto ret = sqlite3_step(m_statement);
    return ret == SQLITE_DONE;
}

bool SQLiteLink::QueryNext()
{
    auto ret = sqlite3_step(m_statement);
    return ret == SQLITE_ROW;
}

bool SQLiteLink::Transaction()
{
    char* errMsg = nullptr;
    auto  ret    = sqlite3_exec(m_database->GetDatabase(), "BEGIN TRANSACTION", nullptr, nullptr, &errMsg);
    if (ret != SQLITE_OK)
    {
        return false;
    }
    return true;
}

bool SQLiteLink::Commit()
{
    char* errMsg = nullptr;
    auto  ret    = sqlite3_exec(m_database->GetDatabase(), "COMMIT", nullptr, nullptr, &errMsg);
    if (ret != SQLITE_OK)
    {
        return false;
    }
    return true;
}

bool SQLiteLink::BindInt(int param, int paramIndex)
{
    auto ret = sqlite3_bind_int(m_statement, paramIndex, param);
    return ret == SQLITE_OK;
}

bool SQLiteLink::BindInt64(Int64 param, int paramIndex)
{
    auto ret = sqlite3_bind_int64(m_statement, paramIndex, param);
    return ret == SQLITE_OK;
}

bool SQLiteLink::BindDouble(double param, int paramIndex)
{
    auto ret = sqlite3_bind_double(m_statement, paramIndex, param);
    return ret == SQLITE_OK;
}

bool SQLiteLink::BindString(const std::string& param, int paramIndex)
{
    auto ret = sqlite3_bind_text(m_statement, paramIndex, param.c_str(), -1, SQLITE_TRANSIENT);
    return ret == SQLITE_OK;
}

int SQLiteLink::ColumnInt(int columnIndex)
{
    return sqlite3_column_int(m_statement, columnIndex);
}

Int64 SQLiteLink::ColumnInt64(int columnIndex)
{
    return sqlite3_column_int64(m_statement, columnIndex);
}

double SQLiteLink::ColumnDouble(int columnIndex)
{
    return sqlite3_column_double(m_statement, columnIndex);
}

std::string SQLiteLink::ColumnString(int columnIndex)
{
    const unsigned char* text = sqlite3_column_text(m_statement, columnIndex);
    if (text)
    {
        return std::string(reinterpret_cast<const char*>(text));
    }
    return std::string();
}
