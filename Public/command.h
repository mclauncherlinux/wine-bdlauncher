#pragma once

#include <string>
#include <string_view>

#include "event.h"
#include <Core/type_id.h>
#include <Command/CommandSelector.h>
#include <Command/CommandRegistry.h>
#include <Command/CommandParameterData.h>

#ifdef CommandSupport_EXPORTS
#  define COMMANDAPI __declspec(dllexport)
#else
#  define COMMANDAPI __declspec(dllimport)
#endif

class CommandRegistry;
class CommandItem;
namespace Json {
class Value;
}

namespace Mod {

class CommandSupport : public EventEmitter<"loaded"_sig, CommandRegistry *> {
  COMMANDAPI CommandSupport();
  static COMMANDAPI short &type_id_count();

public:
  USING_EVENTEMITTER("loaded", CommandRegistry *);
  COMMANDAPI static CommandSupport &GetInstance();

  template <typename T> static typeid_t<CommandRegistry> GetParameterTypeId() {
    static auto value = type_id_count()++;
    return typeid_t<CommandRegistry>{value};
  }
};

template <> COMMANDAPI typeid_t<CommandRegistry> CommandSupport::GetParameterTypeId<bool>();
template <> COMMANDAPI typeid_t<CommandRegistry> CommandSupport::GetParameterTypeId<int>();
template <> COMMANDAPI typeid_t<CommandRegistry> CommandSupport::GetParameterTypeId<float>();
template <> COMMANDAPI typeid_t<CommandRegistry> CommandSupport::GetParameterTypeId<std::string>();
template <> COMMANDAPI typeid_t<CommandRegistry> CommandSupport::GetParameterTypeId<CommandItem>();
template <> COMMANDAPI typeid_t<CommandRegistry> CommandSupport::GetParameterTypeId<Json::Value>();
template <> COMMANDAPI typeid_t<CommandRegistry> CommandSupport::GetParameterTypeId<CommandSelector<Actor>>();
template <> COMMANDAPI typeid_t<CommandRegistry> CommandSupport::GetParameterTypeId<CommandSelector<Player>>();

} // namespace Mod

namespace commands {

template <typename T>
char const *addEnum(
    CommandRegistry *registry, char const *name, std::initializer_list<std::pair<std::string, T>> const &values) {
  registry->addEnumValues<T>(name, Mod::CommandSupport::GetParameterTypeId<T>(), values);
  return name;
}

template <typename Command, typename Type> int getOffset(Type Command::*src) {
  union {
    Type Command::*src;
    int value;
  } u;
  u.src = src;
  return u.value;
}

template <typename Command, typename Type>
CommandParameterData mandatory(Type Command::*field, std::string_view name, bool Command::*isSet = nullptr) {
  return {
      Mod::CommandSupport::GetParameterTypeId<Type>(),
      CommandRegistry::getParseFn<Type>(),
      name,
      CommandParameterDataType::NORMAL,
      nullptr,
      getOffset(field),
      false,
      isSet ? getOffset(isSet) : -1,
  };
}
template <CommandParameterDataType DataType, typename Command, typename Type>
CommandParameterData
mandatory(Type Command::*field, std::string_view name, char const *desc = nullptr, bool Command::*isSet = nullptr) {
  return {
      Mod::CommandSupport::GetParameterTypeId<Type>(),
      &CommandRegistry::fakeparse<Type>,
      name,
      DataType,
      desc,
      getOffset(field),
      false,
      isSet ? getOffset(isSet) : -1,
  };
}
template <typename Command, typename Type>
CommandParameterData optional(Type Command::*field, std::string_view name, bool Command::*isSet = nullptr) {
  return {
      Mod::CommandSupport::GetParameterTypeId<Type>(),
      CommandRegistry::getParseFn<Type>(),
      name,
      CommandParameterDataType::NORMAL,
      nullptr,
      getOffset(field),
      true,
      isSet ? getOffset(isSet) : -1,
  };
}
template <CommandParameterDataType DataType, typename Command, typename Type>
CommandParameterData
optional(Type Command::*field, std::string_view name, char const *desc = nullptr, bool Command::*isSet = nullptr) {
  return {
      Mod::CommandSupport::GetParameterTypeId<Type>(),
      &CommandRegistry::fakeparse<Type>,
      name,
      DataType,
      desc,
      getOffset(field),
      true,
      isSet ? getOffset(isSet) : -1,
  };
}

} // namespace commands