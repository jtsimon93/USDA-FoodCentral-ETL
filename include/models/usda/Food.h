#pragma once

#include <chrono>
#include <optional>
#include <string>

namespace USDA {
enum class FoodDataType { Foundation, Branded };

typedef struct {
  int fdc_id; // Primary key (internal to this table)
  FoodDataType data_type;
  std::string description;
  std::optional<std::string> food_category_id;
  std::chrono::year_month_day publication_date;
} Food;
} // namespace USDA
