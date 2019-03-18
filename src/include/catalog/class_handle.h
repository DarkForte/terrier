#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/catalog.h"
#include "catalog/catalog_defs.h"
#include "catalog/table_handle.h"
#include "storage/sql_table.h"
#include "transaction/transaction_context.h"
namespace terrier::catalog {

class Catalog;
struct SchemaCol;

/**
 * Class (equiv. of pg_class) stores much of the metadata for
 * anything that has columns and is like a table.
 */
class ClassHandle {
 public:
  /**
   * An ClassEntry is a row in pg_class catalog
   */
  class ClassEntry {
   public:
    /**
     * Constructs a Class entry.
     * @param oid
     * @param entry: the row as a vector of values
     */
    ClassEntry(col_oid_t oid, std::vector<type::Value> entry) : oid_(oid), entry_(std::move(entry)) {}

    /**
     * Get the value for a given column
     * @param col_num the column index
     * @return the value of the column
     */
    const type::Value &GetColumn(int32_t col_num) { return entry_[col_num]; }

    /**
     * Return the col_oid of the attribute
     * @return col_oid of the attribute
     */
    col_oid_t GetClassOid() { return oid_; }

   private:
    // the row
    col_oid_t oid_;
    std::vector<type::Value> entry_;
  };

  /**
   * Get a specific Class entry.
   * @param txn the transaction that initiates the read
   * @param oid which entry to return
   * @return a shared pointer to Class entry;
   *         NULL if the entry doesn't exist.
   */
  std::shared_ptr<ClassEntry> GetClassEntry(transaction::TransactionContext *txn, col_oid_t oid);

  /**
   * Add row into the Class table.
   */
  void AddEntry(transaction::TransactionContext *txn, int64_t tbl_ptr, int32_t entry_oid, const std::string &name,
                int32_t ns_oid, int32_t ts_oid);

  /**
   * Constructor
   * @param catalog the global catalog object
   * @param db_oid parent database oid
   * @param table a pointer to SqlTableRW
   * @param pg_attrdef a pointer to pg_attrdef sql table rw helper instance
   */
  explicit ClassHandle(Catalog *catalog, std::shared_ptr<catalog::SqlTableRW> pg_class)
      : catalog_(catalog), pg_class_rw_(std::move(pg_class)) {}

  /**
   * Create the storage table
   */
  static std::shared_ptr<catalog::SqlTableRW> Create(transaction::TransactionContext *txn, Catalog *catalog,
                                                     db_oid_t db_oid, const std::string &name);

  /**
   * Debug methods
   */
  void Dump(transaction::TransactionContext *txn) { pg_class_rw_->Dump(txn); }

  static const std::vector<SchemaCol> schema_cols_;
  static const std::vector<SchemaCol> unused_schema_cols_;

 private:
  Catalog *catalog_;
  // database containing this table
  // db_oid_t db_oid_;
  // storage for this table
  std::shared_ptr<catalog::SqlTableRW> pg_class_rw_;
};
}  // namespace terrier::catalog