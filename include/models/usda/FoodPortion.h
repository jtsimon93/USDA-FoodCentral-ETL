#pragma once

#include <optional>
#include <string>

namespace USDA {
typedef struct {
  int id;              // Primary key of the portion record
  int fdc_id;          // Foreign key to Food.fdc_id
  std::optional<int> seq_num;         // Sequence number (for ordering)
  std::optional<float> amount;        // Quantity of the portion (e.g., 1.0, 0.5)
  std::optional<int> measure_unit_id; // Foreign key to MeasureUnit

  std::optional<std::string> portion_description; // e.g., "1 cup", "1 slice"
  std::optional<std::string> modifier;            // e.g., "chopped", "raw"
  std::optional<float> gram_weight;               // Exact weight in grams
  std::optional<int> data_points;                 // Number of measurements
  std::optional<std::string> footnote;            // Notes or caveats
  std::optional<int> min_year_acquired; // Oldest year data was collected
} FoodPortion;
} // namespace USDA
