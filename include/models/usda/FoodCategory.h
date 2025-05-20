#pragma once

#include <string>

namespace USDA {
typedef struct {
  int id; // Primary key (internal to this table)
  int code;
  std::string description;
} FoodCategory;
} // namespace USDA
