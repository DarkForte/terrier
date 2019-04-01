#include <sqlite3.h>
#include <cstdio>
#include <string>
#include <traffic_cop/sqlite.h>

#include "loggers/main_logger.h"
#include "network/network_defs.h"
#include "traffic_cop/result_set.h"
#include "traffic_cop/sqlite.h"

namespace terrier::traffic_cop {

SqliteEngine::SqliteEngine() {
  auto rc = sqlite3_open_v2("sqlite.db", &sqlite_db_, SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                            nullptr);
  if (rc != 0) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(sqlite_db_));
    LOG_ERROR("Can't open database %s", sqlite3_errmsg(sqlite_db_));
    exit(0);
  } else {
    fprintf(stderr, "\n");
  }
}

SqliteEngine::~SqliteEngine() { sqlite3_close(sqlite_db_); }

void SqliteEngine::ExecuteQuery(const char *query, network::PostgresPacketWriter *out,
                                const network::SimpleQueryCallback &callback) {
  ResultSet result_set;

  sqlite3_exec(sqlite_db_, query, StoreResults, &result_set, &error_msg);
  if (error_msg != nullptr) {
    LOG_ERROR("Error msg from Sqlite3: " + std::string(error_msg));
    sqlite3_free(error_msg);
  }

  callback(result_set, out);
}

int SqliteEngine::StoreResults(void *result_set_void, int elem_count, char **values, char **column_names) {
  auto result_set = reinterpret_cast<ResultSet *>(result_set_void);

  if (result_set->column_names_.empty()) {
    for (int i = 0; i < elem_count; i++) {
      result_set->column_names_.emplace_back(column_names[i]);
    }
  }

  Row current_row;
  for (int i = 0; i < elem_count; i++) {
    current_row.emplace_back(values[i]);
  }

  result_set->rows_.push_back(current_row);

  return 0;
}
sqlite3_stmt *SqliteEngine::PrepareStatement(const char *query) {
  sqlite3_stmt *stmt;
  int error_code = sqlite3_prepare_v2(sqlite_db_, query, -1, &stmt, nullptr);
  if(error_code == SQLITE_OK)
    return stmt;
  else
  {
    LOG_ERROR("Sqlite Prepare Error: Error Code = {0}, msg = {1}", error_code, sqlite3_errmsg(sqlite_db_));
    return nullptr;
  }
}

}  // namespace terrier::traffic_cop