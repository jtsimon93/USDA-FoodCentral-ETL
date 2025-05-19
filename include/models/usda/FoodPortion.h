#pragma once

#include <string>

namespace USDA {
typedef struct {
  int id;
  int fdc_id;
  int seq_num;
  float amount;
  int measure_unit_id;
  std::string portion_description;
  std::string modifier;
  float gram_weight;
} FoodPortion;
} // namespace USDA
