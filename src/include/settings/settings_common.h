// This allows the settings defined once to be used in different contexts.
// When __SETTING_GFLAGS_DEFINE__ is set,
//    setting definitions will be exposed through glfags definitions.
// When __SETTING_GFLAGS_DECLARE__ is set,
//    setting definitions will be exposed through glfags declarations.
// When __SETTING_DEFINE__ is set,
//    setting definitions will be exposed through defitions in settings_manager.
// When __SETTING_ENUM__ is set,
//    setting definitions will be exposed through Param.

#ifdef __SETTING_GFLAGS_DEFINE__

#ifdef SETTING_int
#undef SETTING_int
#endif
#ifdef SETTING_double
#undef SETTING_double
#endif
#ifdef SETTING_bool
#undef SETTING_bool
#endif
#ifdef SETTING_string
#undef SETTING_string
#endif

#define VALIDATOR_int(name, default_value)                          \
  static bool Validate##name(const char *setting_name, int value) { \
    if (FLAGS_##name == static_cast<int>(default_value)) {          \
      return true;                                                  \
    }                                                               \
    SETTINGS_LOG_ERROR(                                             \
        "Value for \"{}"                                            \
        "\" has been set to {})",                                   \
        setting_name, FLAGS_##name);                                \
    return false;                                                   \
  }

#define VALIDATOR_double(name, default_value)                          \
  static bool Validate##name(const char *setting_name, double value) { \
    if (FLAGS_##name == static_cast<double>(default_value)) {          \
      return true;                                                     \
    }                                                                  \
    SETTINGS_LOG_ERROR(                                                \
        "Value for \"{}"                                               \
        "\" has been set to {})",                                      \
        setting_name, FLAGS_##name);                                   \
    return false;                                                      \
  }

#define SETTING_int(name, description, default_value, min_value, max_value, is_mutable, callback_fn) \
  DEFINE_int32(name, default_value, description);                                                    \
  VALIDATOR_int(name, default_value);                                                                \
  DEFINE_validator(name, &Validate##name);

#define SETTING_double(name, description, default_value, min_value, max_value, is_mutable) \
  DEFINE_double(name, default_value, description);                                         \
  VALIDATOR_double(name, default_value);                                                   \
  DEFINE_validator(name, &Validate##name);

#define SETTING_bool(name, description, default_value, is_mutable) DEFINE_bool(name, default_value, description);

#define SETTING_string(name, description, default_value, is_mutable) DEFINE_string(name, default_value, description);
#endif

#ifdef __SETTING_GFLAGS_DECLARE__
#ifdef SETTING_int
#undef SETTING_int
#endif
#ifdef SETTING_double
#undef SETTING_double
#endif
#ifdef SETTING_bool
#undef SETTING_bool
#endif
#ifdef SETTING_string
#undef SETTING_string
#endif

#define SETTING_int(name, description, default_value, min_value, max_value, is_mutable, callback_fn) \
  DECLARE_int32(name);

#define SETTING_double(name, description, default_value, min_value, max_value, is_mutable) DECLARE_double(name);

#define SETTING_bool(name, description, default_value, is_mutable) DECLARE_bool(name);

#define SETTING_string(name, description, default_value, is_mutable) DECLARE_string(name);
#endif

#ifdef __SETTING_DEFINE__
#ifdef SETTING_int
#undef SETTING_int
#endif
#ifdef SETTING_double
#undef SETTING_double
#endif
#ifdef SETTING_bool
#undef SETTING_bool
#endif
#ifdef SETTING_string
#undef SETTING_string
#endif
#define SETTING_int(name, description, default_value, min_value, max_value, is_mutable, callback_fn)          \
  DefineSetting(terrier::settings::Param::name, #name, type::TransientValueFactory::GetInteger(FLAGS_##name), \
                description, type::TransientValueFactory::GetInteger(default_value),                          \
                type::TransientValueFactory::GetInteger(min_value),                                           \
                type::TransientValueFactory::GetInteger(max_value), is_mutable, callback_fn);

#define SETTING_double(name, description, default_value, min_value, max_value, is_mutable)                    \
  DefineSetting(terrier::settings::Param::name, #name, type::TransientValueFactory::GetDecimal(FLAGS_##name), \
                description, type::TransientValueFactory::GetDecimal(default_value),                          \
                type::TransientValueFactory::GetDecimal(min_value),                                           \
                type::TransientValueFactory::GetDecimal(max_value), is_mutable);

#define SETTING_bool(name, description, default_value, is_mutable)                                            \
  DefineSetting(terrier::settings::Param::name, #name, type::TransientValueFactory::GetBoolean(FLAGS_##name), \
                description, type::TransientValueFactory::GetBoolean(default_value),                          \
                type::TransientValueFactory::GetBoolean(default_value),                                       \
                type::TransientValueFactory::GetBoolean(default_value), is_mutable);

#define SETTING_string(name, description, default_value, is_mutable)                                          \
  DefineSetting(terrier::settings::Param::name, #name, type::TransientValueFactory::GetVarchar(FLAGS_##name), \
                description, type::TransientValueFactory::GetVarchar(default_value),                          \
                type::TransientValueFactory::GetVarchar(default_value),                                       \
                type::TransientValueFactory::GetVarchar(default_value), is_mutable);
#endif

#ifdef __SETTING_ENUM__
#ifdef SETTING_int
#undef SETTING_int
#endif
#ifdef SETTING_double
#undef SETTING_double
#endif
#ifdef SETTING_bool
#undef SETTING_bool
#endif
#ifdef SETTING_string
#undef SETTING_string
#endif
#define SETTING_int(name, description, default_value, min_value, max_value, is_mutable, callback_fn) name,

#define SETTING_double(name, description, default_value, min_value, max_value, is_mutable) name,

#define SETTING_bool(name, description, default_value, is_mutable) name,

#define SETTING_string(name, description, default_value, is_mutable) name,
#endif