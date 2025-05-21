#pragma once

#include "models/usda/FoodPortion.h"
#include <string>
#include <vector>

/**
 * @class FoodPortionExtractorService
 * @brief Extracts food portion information from the USDA Food Data Central CSV file.
 *
 * This service parses the food_portion.csv file to extract information about
 * standard food portion sizes and their gram weights. Food portions define
 * serving sizes, household measures, and their equivalent gram weights for foods.
 */
class FoodPortionExtractorService {
public:
  /**
   * @brief Constructs a FoodPortionExtractorService with the specified input file.
   *
   * @param food_portion_input_file Path to the food_portion.csv input file
   */
  FoodPortionExtractorService(const std::string &food_portion_input_file);
  ~FoodPortionExtractorService() = default;

  /**
   * @brief Retrieves the extracted food portion entries.
   *
   * Returns a reference
   * to the internal vector of food portion entries.
   *
   * @return Reference to vector of USDA::FoodPortion objects
   */
  std::vector<USDA::FoodPortion> &GetFoodPortionEntries();

private:
  /**
   * @brief Parses the food_portion.csv file and populates the food_portion_entries vector.
   */
  void ExtractFoodPortionEntries();

  std::string food_portion_input_file; ///< Path to the food portion CSV input file
  std::vector<USDA::FoodPortion> food_portion_entries; ///< Storage for extracted food portion entries
};
