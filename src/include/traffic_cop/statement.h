#pragma once

#include <sqlite3.h>
#include <vector>
#include "type/transient_value.h"
#include "network/postgres_protocol_utils.h"

namespace terrier::traffic_cop {

struct Statement {

  sqlite3_stmt *sqlite3_stmt_;
  std::vector<type::TransientValue> query_params;

};

}
