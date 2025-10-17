#pragma once
#include "DataPersistence/IDatabase.h"
#include <sqlite3.h>

class SQLiteDatabase : public IDatabase
{
  public:
    SQLiteDatabase();
    virtual ~SQLiteDatabase();

    virtual bool Open(const std::string& databaePath) override;
    virtual void Close() override;

    virtual DisposableLink GetDisposableLink() override;

    sqlite3* GetDatabase();

  private:
    sqlite3* m_database = nullptr;
};
