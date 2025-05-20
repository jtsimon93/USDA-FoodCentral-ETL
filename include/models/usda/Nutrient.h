#pragma once

#include <optional>
#include <string>

namespace USDA {
typedef struct {
  int id;                // Primary key (used in food_nutrient)
  std::string name;      // Nutrient name (e.g., "Protein")
  std::string unit_name; // Unit (e.g., "g", "mg", "µg")
  std::optional<std::string> nutrient_nbr; // Legacy ID, sometimes blank
  std::optional<int> rank;                 // Display or sort order
} Nutrient;
} // namespace USDA
