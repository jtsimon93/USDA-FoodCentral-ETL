#include "services/transformers/ValidFDCIDTransformer.h"
#include <iostream>
#include <unordered_set>

/**
 * Filters food nutrient and food portion entries to ensure they all reference 
 * valid Food Data Central IDs (FDC IDs) from the master food entries list.
 * 
 * During the extraction process, only foundation and branded foods are retained
 * in the food entries, while sample foods and sub-sample foods are skipped. 
 * This transformation removes any orphaned records in related tables that 
 * reference skipped FDC IDs.
 * 
 * @param food_entries Master list of food entries with valid FDC IDs
 * @param food_nutrient_entries Food nutrient entries to be filtered
 * @param food_portion_entries Food portion entries to be filtered
 */
void ValidFDCIDTransformer::TransformData(
    std::vector<USDA::Food> &food_entries,
    std::vector<USDA::FoodNutrient> &food_nutrient_entries,
    std::vector<USDA::FoodPortion> &food_portion_entries) {
  std::cout << "Starting Valid FDC ID Transform...\n";
  const auto start_time = std::chrono::high_resolution_clock::now();
  int total_transformed_entries = 0;

  // Create a hash set of all valid food IDs for efficient lookup
  std::unordered_set<int> valid_fdc_ids;
  valid_fdc_ids.reserve(food_entries.size());
  for (const auto &food : food_entries) {
    valid_fdc_ids.insert(food.fdc_id);
  }

  // Filter food nutrient entries, removing those without a corresponding valid FDC ID
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
  total_transformed_entries += removed_food_nutrient_count;

  // Filter food portion entries, removing those without a corresponding valid FDC ID
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
  total_transformed_entries += removed_food_portion_count;

  // Calculate and report timing and transformation statistics
  const auto end_time = std::chrono::high_resolution_clock::now();
  const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);
  std::cout << "Time taken to transform all entries: " << duration.count()
            << " milliseconds.\n";
  std::cout << "Total removed entries: " << total_transformed_entries
            << "\n";
}
