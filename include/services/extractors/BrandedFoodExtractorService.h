#pragma once

#include "models/usda/BrandedFood.h"
#include <string>
#include <vector>

/**
 * @class BrandedFoodExtractorService
 * @brief Extracts branded food information from the USDA Food Data Central CSV file.
 *
 * This service parses the branded_food.csv file to extract information about
 * commercial food products, including brand names, ingredients, serving sizes,
 * and other brand-specific attributes. Branded foods represent commercially
 * available products in the food database.
 */
class BrandedFoodExtractorService {

public:
  /**
   * @brief Constructs a BrandedFoodExtractorService with the specified input file.
   *
   * @param branded_food_input_file Path to the branded_food.csv input file
   */
  BrandedFoodExtractorService(const std::string &branded_food_input_file);
  ~BrandedFoodExtractorService() = default;

  /**
   * @brief Retrieves the extracted branded food entries.
   *
   * Calls ExtractBrandedFoodEntries() if not already called, then returns a reference
   * to the internal vector of branded food entries.
   *
   * @return Reference to vector of USDA::BrandedFood objects
   */
  std::vector<USDA::BrandedFood> &GetBrandedFoodEntries();

private:
  /**
   * @brief Parses the branded_food.csv file and populates the branded_food_entries vector.
   */
  void ExtractBrandedFoodEntries();

  std::string branded_food_input_file; ///< Path to the branded food CSV input file
  std::vector<USDA::BrandedFood> branded_food_entries; ///< Storage for extracted branded food entries
};
