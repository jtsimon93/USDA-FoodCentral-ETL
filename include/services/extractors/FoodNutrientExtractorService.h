#pragma once

#include "models/usda/FoodNutrient.h"
#include <string>
#include <vector>

/**
 * @class FoodNutrientExtractorService
 * @brief Extracts food nutrient data from the USDA Food Data Central CSV file.
 *
 * This service parses the food_nutrient.csv file to extract nutritional data
 * that links foods to their nutrient values. Each FoodNutrient entry connects 
 * a specific food item to a nutrient and its corresponding amount.
 */
class FoodNutrientExtractorService {
public:
  /**
   * @brief Constructs a FoodNutrientExtractorService with the specified input file.
   *
   * @param food_nutrient_input_file Path to the food_nutrient.csv input file
   */
  FoodNutrientExtractorService(const std::string &food_nutrient_input_file);
  ~FoodNutrientExtractorService() = default;

  /**
   * @brief Retrieves the extracted food nutrient entries.
   *
   * Calls ExtractFoodNutrientEntries() if not already called, then returns a reference
   * to the internal vector of food nutrient entries.
   *
   * @return Reference to vector of USDA::FoodNutrient objects
   */
  std::vector<USDA::FoodNutrient> &GetFoodNutrientEntries();

private:
  /**
   * @brief Parses the food_nutrient.csv file and populates the food_nutrient_entries vector.
   */
  void ExtractFoodNutrientEntries();

  std::string food_nutrient_input_file; ///< Path to the food nutrient CSV input file
  std::vector<USDA::FoodNutrient> food_nutrient_entries; ///< Storage for extracted food nutrient entries
};
