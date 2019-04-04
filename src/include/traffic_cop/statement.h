#pragma once

#include <sqlite3.h>
#include <vector>
#include "network/postgres_protocol_utils.h"
#include "type/transient_value.h"

namespace terrier::traffic_cop {

/**
 * Statement contains a prepared statement by the backend.
 */
struct Statement {
  /* The sqlite3 statement */
  sqlite3_stmt *sqlite3_stmt_;

  /* The types of the parameters*/
  std::vector<type::TypeId> param_types;

  /**
   * Return the number of the parameters.
   * @return
   */
  size_t NumParams() { return param_types.size(); }
};

}  // namespace terrier::traffic_cop