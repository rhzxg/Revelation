#pragma once
#include "IRevelationDataDefine.h"
#include <memory>

enum class DatabaseType
{
    Invalid = 0,
    SQLite,
};

enum class DatabaseRole
{
    Unknown = 0,
    Primary,
    Daily,
};

class IDatabaseLink;
using DisposableLink = std::shared_ptr<IDatabaseLink>;

class IDatabase
{
  public:
    IDatabase(){};
    virtual ~IDatabase(){};

    virtual bool Open(const std::string& databaePath) = 0;
    virtual void Close()                              = 0;

    virtual DisposableLink GetDisposableLink() = 0;

    virtual void SetDatabaseRole(DatabaseRole role)
    {
        m_databaseRole = role;
    }

    virtual DatabaseRole GetDatabaseRole()
    {
        return m_databaseRole;
    }

  private:
    DatabaseRole m_databaseRole = DatabaseRole::Unknown;
};

class IDatabaseLink
{
  public:
    IDatabaseLink() {}
    virtual ~IDatabaseLink(){};

    virtual bool Exec(const std::string& stmt)        = 0;
    virtual bool PrepareStmt(const std::string& stmt) = 0;
    virtual bool Evaluate()                           = 0;
    virtual bool QueryNext()                          = 0;

    virtual bool Transaction() = 0;
    virtual bool Commit()      = 0;

    virtual bool BindInt(int param, int paramIndex)                   = 0;
    virtual bool BindInt64(Int64 param, int paramIndex)               = 0;
    virtual bool BindDouble(double param, int paramIndex)             = 0;
    virtual bool BindString(const std::string& param, int paramIndex) = 0;

    virtual int         ColumnInt(int columnIndex)    = 0;
    virtual Int64       ColumnInt64(int columnIndex)  = 0;
    virtual double      ColumnDouble(int columIndex)  = 0;
    virtual std::string ColumnString(int columnIndex) = 0;
};
