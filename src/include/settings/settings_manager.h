#pragma once

#include <catalog/settings_handle.h>
#include <gflags/gflags.h>
#include <unordered_map>
#include "common/exception.h"
#include "loggers/settings_logger.h"
#include "main/main_database.h"
#include "settings/settings_param.h"
#include "type/value.h"

#define __SETTING_GFLAGS_DECLARE__
#include "settings/settings_macro.h"
#include "settings/settings.h"
#undef __SETTING_GFLAGS_DECLARE__

namespace terrier::settings {

using callback_fn = void (*)(void *, void *);

/*
 * SettingsManager:
 * A wrapper for pg_settings table, does not store values in it.
 * Stores and triggers callbacks when a tunable parameter is changed.
 * Static class.
 */

class SettingsManager {
 public:
  SettingsManager() = delete;
  SettingsManager(const SettingsManager &) = delete;
  SettingsManager(const std::shared_ptr<catalog::Catalog> &catalog, transaction::TransactionManager *txn_manager);

  int32_t GetInt(Param param);
  double GetDouble(Param param);
  bool GetBool(Param param);
  std::string GetString(Param param);

  void SetInt(Param param, int32_t value);
  void SetBool(Param param, bool value);
  void SetString(Param param, const std::string &value);

  // Call this method in Catalog->Bootstrap
  // to store information into pg_settings
  void InitializeCatalog();

  const std::string GetInfo();

  void ShowInfo();

  void InitParams();

 private:
  catalog::SettingsHandle settings_handle_;
  transaction::TransactionManager *txn_manager_;
  std::unordered_map<Param, ParamInfo> param_map_;
  std::unordered_map<Param, callback_fn> callback_map_;

  void DefineSetting(Param param, const std::string &name, const type::Value &value, const std::string &description,
                     const type::Value &default_value, const type::Value &min_value, const type::Value &max_value,
                     bool is_mutable, callback_fn callback = nullptr);

  type::Value GetValue(Param param);
  void SetValue(Param param, const type::Value &value);
};

}  // namespace terrier::settings
