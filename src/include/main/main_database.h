#pragma once

#include <gflags/gflags.h>
#include <network/terrier_server.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include "bwtree/bwtree.h"
#include "catalog/catalog.h"
#include "common/allocator.h"
#include "common/stat_registry.h"
#include "common/strong_typedef.h"
#include "loggers/catalog_logger.h"
#include "loggers/index_logger.h"
#include "loggers/main_logger.h"
#include "loggers/network_logger.h"
#include "loggers/parser_logger.h"
#include "loggers/storage_logger.h"
#include "loggers/transaction_logger.h"
#include "loggers/type_logger.h"
#include "settings/settings_manager.h"
#include "storage/data_table.h"
#include "storage/record_buffer.h"
#include "storage/storage_defs.h"
#include "transaction/transaction_context.h"
#include "transaction/transaction_manager.h"

namespace terrier {

/**
 * The MainDatabase Class holds all the static functions, singleton pointers, etc.
 * It has the full knowledge of the whole database systems.
 */

class MainDatabase {
 public:
  /**
   * Bootstrap the whole database and running the main thread.
   * @param argc the number of command line arguments
   * @param argv a list of command line arguments
   * @return exit status
   */
  static int start(int argc, char *argv[]);

  /**
   * Basic empty callbacks used by settings manager
   * @param old_value the old value of corresponding setting
   * @param new_value the new value of corresponding setting
   */
  static void EmptyCallback(void *old_value UNUSED_ATTRIBUTE, void *new_value UNUSED_ATTRIBUTE);

  /**
   * Buffer pool size callback used by settings manager
   * @param old_value old value of buffer pool size
   * @param new_value new value of buffer pool size
   */
  static void BufferPoolSizeCallback(void *old_value UNUSED_ATTRIBUTE, void *new_value UNUSED_ATTRIBUTE);

  static transaction::TransactionManager* txn_manager_;
};

}  // namespace terrier
