#pragma once

#include "models/usda/Food.h"
#include "models/usda/FoodNutrient.h"
#include "models/usda/FoodPortion.h"

/**
 * @brief Transformer that ensures all entries reference valid Food Data Central IDs.
 *
 * This class provides functionality to filter food-related data collections,
 * removing entries that reference Food Data Central IDs (FDC IDs) not present
 * in the master food entries collection. This ensures data consistency across
 * the different USDA food data tables.
 */
class ValidFDCIDTransformer {
public:
  ValidFDCIDTransformer() = default;
  ~ValidFDCIDTransformer() = default;

  /**
   * @brief Filters food nutrient and portion entries to ensure valid FDC ID references.
   *
   * This static method removes entries from food_nutrient_entries and food_portion_entries
   * that reference FDC IDs not present in food_entries. During extraction, certain
   * food types (e.g., sample foods) are excluded from food_entries, but related entries
   * might still exist in other collections. This method ensures consistency by removing
   * those orphaned references.
   *
   * @param food_entries The valid food entries containing the set of legitimate FDC IDs
   * @param food_nutrient_entries Collection of food nutrient entries to be filtered
   * @param food_portion_entries Collection of food portion entries to be filtered
   */
  static void TransformData(std::vector<USDA::Food> &food_entries,
                     std::vector<USDA::FoodNutrient> &food_nutrient_entries,
                     std::vector<USDA::FoodPortion> &food_portion_entries);
};
