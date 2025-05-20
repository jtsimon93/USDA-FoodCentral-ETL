#pragma once

#include <string>

namespace USDA {
typedef struct {
  int id; // Primary key (internal to this table)
  std::string name;
} MeasureUnit;
} // namespace USDA
