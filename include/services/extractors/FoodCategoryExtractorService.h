#pragma once

#include "models/usda/FoodCategory.h"
#include <string>
#include <vector>

/**
 * @class FoodCategoryExtractorService
 * @brief Extracts food category data from the USDA Food Data Central CSV file.
 *
 * This service parses the food_category.csv file to extract category information
 * used to classify and organize food entries. Food categories define the taxonomic
 * groupings of food items in the database.
 */
class FoodCategoryExtractorService {
public:
  /**
   * @brief Constructs a FoodCategoryExtractorService with the specified input file.
   *
   * @param food_category_input_file Path to the food_category.csv input file
   */
  FoodCategoryExtractorService(const std::string &food_category_input_file);
  ~FoodCategoryExtractorService() = default;

  /**
   * @brief Retrieves the extracted food category entries.
   *
   * Calls ExtractFoodCategoryEntries() if not already called, then returns a reference
   * to the internal vector of food category entries.
   *
   * @return Reference to vector of USDA::FoodCategory objects
   */
  std::vector<USDA::FoodCategory> &GetFoodCategoryEntries();

private:
  /**
   * @brief Parses the food_category.csv file and populates the food_category_entries vector.
   */
  void ExtractFoodCategoryEntries();

  std::string food_category_input_file; ///< Path to the food category CSV input file
  std::vector<USDA::FoodCategory> food_category_entries; ///< Storage for extracted food category entries
};
