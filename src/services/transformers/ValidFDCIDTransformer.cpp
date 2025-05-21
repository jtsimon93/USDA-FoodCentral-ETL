#include "services/transformers/ValidFDCIDTransformer.h"
#include <iostream>
#include <unordered_set>

void ValidFDCIDTransformer::TransformData(
    std::vector<USDA::Food> &food_entries,
    std::vector<USDA::FoodNutrient> &food_nutrient_entries,
    std::vector<USDA::FoodPortion> &food_portion_entries) {
  std::cout << "Starting Valid FDC ID Transform...\n";

  // Use a hash set for O(1) lookup efficiency when filtering entries
  std::unordered_set<int> valid_fdc_ids;
  valid_fdc_ids.reserve(food_entries.size());
  for (const auto &food : food_entries) {
    valid_fdc_ids.insert(food.fdc_id);
  }

  // Filter food nutrient entries using the erase-remove idiom for optimal
  // performance
  const auto initial_food_nutrient_size = food_nutrient_entries.size();
  food_nutrient_entries.erase(
      std::remove_if(food_nutrient_entries.begin(), food_nutrient_entries.end(),
                     [&valid_fdc_ids](const USDA::FoodNutrient &fn) {
                       return valid_fdc_ids.find(fn.fdc_id) ==
                              valid_fdc_ids.end();
                     }),
      food_nutrient_entries.end());
  const auto final_food_nutrient_size = food_nutrient_entries.size();
  const auto removed_food_nutrient_count =
      initial_food_nutrient_size - final_food_nutrient_size;

  // Apply the same filtering algorithm to food portion entries
  const auto initial_food_portion_size = food_portion_entries.size();
  food_portion_entries.erase(
      std::remove_if(food_portion_entries.begin(), food_portion_entries.end(),
                     [&valid_fdc_ids](const USDA::FoodPortion &fp) {
                       return valid_fdc_ids.find(fp.fdc_id) ==
                              valid_fdc_ids.end();
                     }),
      food_portion_entries.end());
  const auto final_food_portion_size = food_portion_entries.size();
  const auto removed_food_portion_count =
      initial_food_portion_size - final_food_portion_size;

  // Transformation statistics
  std::cout << "Removed " << removed_food_nutrient_count
            << " food nutrient entries with invalid FDC IDs\n";
  std::cout << "Removed " << removed_food_portion_count
            << " food portion entries with invalid FDC IDs\n\n";

  // Optimize memory usage
  food_nutrient_entries.shrink_to_fit();
  food_portion_entries.shrink_to_fit();
}
