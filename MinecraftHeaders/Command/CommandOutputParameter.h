#pragma once

#include <string>
#include <vector>
#include "../Core/BlockPos.h"
#include "../Actor/Actor.h"

class Player;

class CommandOutputParameter {
  std::string str;
  int type;

public:
  __declspec(dllimport) CommandOutputParameter(std::string const &);
  __declspec(dllimport) CommandOutputParameter(int);
  __declspec(dllimport) CommandOutputParameter(BlockPos);
  __declspec(dllimport) CommandOutputParameter(std::vector<Player const *> const &);
  __declspec(dllimport) CommandOutputParameter(std::vector<std::string> const &);

  inline CommandOutputParameter(std::string str, int type) : str(str), type(type) {}

  inline CommandOutputParameter(Actor const *actor) : str(actor->getEntityName()), type(1) {}
  inline CommandOutputParameter(Actor const &actor) : str(actor.getEntityName()), type(1) {}
  inline CommandOutputParameter(char const *str) : str(str), type(0) {}
};