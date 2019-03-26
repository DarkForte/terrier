#include "catalog/settings_handle.h"
#include <algorithm>
#include <random>
#include <vector>
#include "catalog/catalog.h"
#include "transaction/transaction_manager.h"
#include "util/test_harness.h"
#include "util/transaction_test_util.h"
namespace terrier {

struct SettingsHandleTests : public TerrierTest {
  void SetUp() override {
    TerrierTest::SetUp();
    txn_manager_ = new transaction::TransactionManager(&buffer_pool_, true, LOGGING_DISABLED);

    catalog_ = new catalog::Catalog(txn_manager_);
    txn_ = txn_manager_->BeginTransaction();
  }

  void TearDown() override {
    txn_manager_->Commit(txn_, TestCallbacks::EmptyCallback, nullptr);

    TerrierTest::TearDown();
    delete catalog_;  // delete catalog first
    delete txn_manager_;
    delete txn_;
  }

  catalog::Catalog *catalog_;
  storage::RecordBufferSegmentPool buffer_pool_{100, 100};

  transaction::TransactionContext *txn_;
  transaction::TransactionManager *txn_manager_;
};

// NOLINTNEXTLINE
TEST_F(SettingsHandleTests, BasicTest) {
  auto settings_handle = catalog_->GetSettingsHandle();

  // create an entry
  std::vector<type::Value> s1;

  const catalog::settings_oid_t s_oid(3);
  s1.emplace_back(type::ValueFactory::GetIntegerValue(!s_oid));
  s1.emplace_back(type::ValueFactory::GetVarcharValue("test_setting_name"));
  s1.emplace_back(type::ValueFactory::GetVarcharValue("test_setting"));
  for (int32_t i = 0; i < 15; i++) {
    s1.emplace_back(type::ValueFactory::GetNullValue(type::TypeId::VARCHAR));
  }
  // source line
  s1.emplace_back(type::ValueFactory::GetIntegerValue(7));
  s1.emplace_back(type::ValueFactory::GetBooleanValue(false));
  settings_handle.InsertRow(txn_, s1);

  // verify correctly created
  auto entry = settings_handle.GetSettingsEntry(txn_, "test_setting_name");
  EXPECT_EQ(3, entry->GetColumn(0).GetIntValue());
  EXPECT_STREQ("test_setting", entry->GetColumn(2).GetVarcharValue());
  EXPECT_EQ(7, entry->GetColumn(18).GetIntValue());
}
}  // namespace terrier