#pragma once

#include <chrono>
#include <optional>
#include <string>

namespace USDA {
typedef struct {
  int fdc_id; // foreign key to Food.fdc_id

  std::optional<std::string> brand_owner;
  std::optional<std::string> brand_name;
  std::optional<std::string> subbrand_name;
  std::optional<std::string> gtin_upc;
  std::optional<std::string> ingredients;
  std::optional<std::string> not_a_significant_source_of;

  std::optional<float> serving_size;
  std::optional<std::string> serving_size_unit;
  std::optional<std::string> household_serving_fulltext;

  std::optional<std::string> branded_food_category;
  std::optional<std::string> data_source;

  std::optional<std::string> package_weight;

  std::optional<std::chrono::year_month_day> modified_date;
  std::optional<std::chrono::year_month_day> available_date;
  std::optional<std::chrono::year_month_day> discontinued_date;

  std::optional<std::string> market_country;
  std::optional<std::string> preparation_state_code;
  std::optional<std::string> trade_channel;
  std::optional<std::string> short_description;
  std::optional<std::string> material_code;
} BrandedFood;
} // namespace USDA
