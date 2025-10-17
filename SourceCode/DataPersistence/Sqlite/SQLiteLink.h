#pragma once
#include "SQLiteDatabase.h"

class SQLiteLink : public IDatabaseLink
{
  public:
    SQLiteLink(SQLiteDatabase* database);
    ~SQLiteLink();

    virtual bool PrepareStmt(const std::string& stmt) override;
    virtual bool Exec(const std::string& stmt) override;
    virtual bool Evaluate() override;
    virtual bool QueryNext() override;

    virtual bool Transaction() override;
    virtual bool Commit() override;

    virtual bool BindInt(int param, int paramIndex) override;
    virtual bool BindInt64(Int64 param, int paramIndex) override;
    virtual bool BindDouble(double param, int paramIndex) override;
    virtual bool BindString(const std::string& param, int paramIndex) override;

    virtual int         ColumnInt(int columnIndex) override;
    virtual Int64       ColumnInt64(int columnIndex) override;
    virtual double      ColumnDouble(int columIndex) override;
    virtual std::string ColumnString(int columnIndex) override;

  private:
    SQLiteDatabase* m_database  = nullptr;
    sqlite3_stmt*   m_statement = nullptr;
};
