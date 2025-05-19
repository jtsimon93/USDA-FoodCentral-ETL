#pragma once

#include <chrono>
#include <string>

namespace USDA {
typedef struct {
  int fdc_id;
  std::string data_type;
  std::string description;
  int food_category_id;
  std::chrono::year_month_day publication_date;
} Food;
} // namespace USDA
