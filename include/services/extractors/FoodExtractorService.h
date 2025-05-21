#pragma once
#include "models/usda/Food.h"
#include <string>
#include <vector>

/**
 * @class FoodExtractorService
 * @brief Service for extracting food data from USDA CSV files.
 *
 * This service parses the USDA Food Central food.csv file and extracts food entries.
 * It filters the input to only include foundation and branded foods, skipping
 * other food types such as sample foods and sub-sample foods.
 */
class FoodExtractorService {
public:
  /**
   * @brief Constructs a FoodExtractorService with the specified input file.
   *
   * @param food_input_file Path to the USDA food.csv input file
   */
  FoodExtractorService(const std::string &food_input_file);
  ~FoodExtractorService() = default;

  /**
   * @brief Retrieves the extracted food entries.
   *
   * Returns a reference
   * to the internal vector of food entries.
   *
   * @return Reference to vector of USDA::Food objects
   */
  std::vector<USDA::Food> &GetFoodEntries();

private:
  /**
   * @brief Parses the food.csv file and populates the food_entries vector.
   *
   * Only extracts foundation_food and branded_food types, filtering out
   * other food types like sample foods.
   */
  void ExtractFoodEntries();

  std::string food_input_file; ///< Path to the food CSV input file
  std::vector<USDA::Food> food_entries; ///< Storage for extracted food entries
};
