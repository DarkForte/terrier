#include "main/db_main.h"
#include <memory>
#include "loggers/loggers_util.h"
#include "settings/settings_manager.h"
#include "storage/garbage_collector_thread.h"
#include "transaction/transaction_manager.h"
#include "transaction/transaction_util.h"

namespace terrier {

void DBMain::Init() {
  LoggersUtil::Initialize(false);

  // initialize stat registry
  main_stat_reg_ = std::make_shared<common::StatisticsRegistry>();

  // create the global transaction mgr
  buffer_segment_pool_ = new storage::RecordBufferSegmentPool(
      type::TransientValuePeeker::PeekInteger(param_map_.find(settings::Param::buffer_pool_size)->second.value_),
      10000);
  txn_manager_ = new transaction::TransactionManager(buffer_segment_pool_, true, nullptr);
  // TODO(Matt): gc interval should come from the settings manager
  gc_thread_ = new storage::GarbageCollectorThread(txn_manager_, std::chrono::milliseconds{10});
  transaction::TransactionContext *txn = txn_manager_->BeginTransaction();
  // create the (system) catalogs
  catalog_ = new catalog::Catalog(txn_manager_, txn);
  settings_manager_ = new settings::SettingsManager(this, catalog_);
  txn_manager_->Commit(txn, transaction::TransactionUtil::EmptyCallback, nullptr);
  LOG_INFO("Initialization complete");

  initialized = true;
}

void DBMain::Run() {
  running = true;
  server_.SetPort(static_cast<int16_t>(
      type::TransientValuePeeker::PeekInteger(param_map_.find(settings::Param::port)->second.value_)));
  server_.SetupServer().ServerLoop();

  // server loop exited, begin cleaning up
  CleanUp();
}

void DBMain::ForceShutdown() {
  if (running) {
    server_.Close();
  }
  CleanUp();
}

void DBMain::CleanUp() {
  main_stat_reg_->Shutdown(false);
  LOG_INFO("Terrier has shut down.");

  LoggersUtil::ShutDown();
}

}  // namespace terrier
