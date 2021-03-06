#pragma once

#include <string>
#include "base.h"
#include <boost/format.hpp>
#include <boost/core/noncopyable.hpp>

namespace Mods {

class Logger : public boost::noncopyable {
  char const *source;
  char const *name;

public:
  enum class Level {
    Verbose = 1,
    Info    = 2,
    Warning = 4,
    Error   = 8,
  };

private:
  BASEAPI void commit(Level level, unsigned line, std::string value);

public:
  class Commit : public boost::noncopyable {
    Logger &logger;
    Level level;
    unsigned line;
    boost::format format;
    friend class Logger;
    Commit(Logger &logger, Level level, unsigned line, char const *pattern)
        : logger(logger), level(level), line(line), format(pattern) {}

  public:
    template <typename T> Commit &operator%(T val) {
      format % val;
      return *this;
    }
    ~Commit() { logger.commit(level, line, format.str()); }
  };

  inline Logger(char const *source, char const *name) : source(source), name(name) {}

  Commit log(Level level, unsigned line, char const *pattern) { return {*this, level, line, pattern}; }
};

} // namespace Mods

#define __STRINGIFY_AUX(x) #x
#define __STRINGIFY(x) __STRINGIFY_AUX(x)
#define DEF_LOGGER(name)                                                                                               \
  static ::Mods::Logger _logger { "Mod::" __STRINGIFY(MODNAME), name }
#define LOGV(content) _logger.log(::Mods::Logger::Level::Verbose, __LINE__, content)
#define LOGI(content) _logger.log(::Mods::Logger::Level::Info, __LINE__, content)
#define LOGW(content) _logger.log(::Mods::Logger::Level::Warning, __LINE__, content)
#define LOGE(content) _logger.log(::Mods::Logger::Level::Error, __LINE__, content)
