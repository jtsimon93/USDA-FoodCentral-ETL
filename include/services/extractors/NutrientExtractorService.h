#pragma once

#include "models/usda/Nutrient.h"
#include <string>
#include <vector>

/**
 * @class NutrientExtractorService
 * @brief Extracts nutrient information from the USDA Food Data Central CSV file.
 *
 * This service parses the nutrient.csv file to extract information about nutritional
 * components like vitamins, minerals, macronutrients, etc. Nutrient entries provide
 * the reference data for food nutrient values and include details such as name,
 * unit of measurement, and rank.
 */
class NutrientExtractorService {
public:
  /**
   * @brief Constructs a NutrientExtractorService with the specified input file.
   *
   * @param nutrient_input_file Path to the nutrient.csv input file
   */
  NutrientExtractorService(const std::string &nutrient_input_file);
  ~NutrientExtractorService() = default;

  /**
   * @brief Retrieves the extracted nutrient entries.
   *
   * Calls ExtractNutrientEntries() if not already called, then returns a reference
   * to the internal vector of nutrient entries.
   *
   * @return Reference to vector of USDA::Nutrient objects
   */
  std::vector<USDA::Nutrient> &GetNutrientEntries();

private:
  /**
   * @brief Parses the nutrient.csv file and populates the nutrient_entries vector.
   */
  void ExtractNutrientEntries();

  std::string nutrient_input_file; ///< Path to the nutrient CSV input file
  std::vector<USDA::Nutrient> nutrient_entries; ///< Storage for extracted nutrient entries
};
