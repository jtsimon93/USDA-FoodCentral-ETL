#pragma once

#include <string>

namespace USDA {
typedef struct {
  int id;
  std::string name;
  std::string unit_name;
  float nutrient_nbr;
  float rank;
} Nutrient;
} // namespace USDA
