
#pragma once
#include <optional>
#include <string>

namespace USDA {
typedef struct {
  int id;          // Primary key (internal to this table)
  int fdc_id;      // Foreign key to Food.fdc_id
  int nutrient_id; // Foreign key to Nutrient.id
  std::optional<float> amount;    // Amount of the nutrient in 100g of food

  // Optional metadata
  std::optional<int> data_points;
  std::optional<std::string> derivation_id;
  std::optional<float> min;
  std::optional<float> max;
  std::optional<float> median;
  std::optional<float> loq;
  std::optional<std::string> footnote;
  std::optional<int> min_year_acquired;
  std::optional<float> percent_daily_value;
} FoodNutrient;
} // namespace USDA
