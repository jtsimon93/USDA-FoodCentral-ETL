#pragma once

#include "models/usda/MeasureUnit.h"
#include <string>
#include <vector>

/**
 * @class MeasureUnitExtractorService
 * @brief Extracts measurement unit information from the USDA Food Data Central CSV file.
 *
 * This service parses the measure_unit.csv file to extract information about 
 * units of measurement used for food portions (e.g., cup, tbsp, g, oz).
 * These measurement units are referenced by food portion entries.
 */
class MeasureUnitExtractorService {
public:
  /**
   * @brief Constructs a MeasureUnitExtractorService with the specified input file.
   *
   * @param measure_unit_input_file Path to the measure_unit.csv input file
   */
  MeasureUnitExtractorService(const std::string &measure_unit_input_file);
  ~MeasureUnitExtractorService() = default;

  /**
   * @brief Retrieves the extracted measure unit entries.
   *
   * Calls ExtractMeasureUnitEntries() if not already called, then returns a reference
   * to the internal vector of measure unit entries.
   *
   * @return Reference to vector of USDA::MeasureUnit objects
   */
  std::vector<USDA::MeasureUnit> &GetMeasureUnitEntries();

private:
  /**
   * @brief Parses the measure_unit.csv file and populates the measure_unit_entries vector.
   */
  void ExtractMeasureUnitEntries();

  std::string measure_unit_input_file; ///< Path to the measure unit CSV input file
  std::vector<USDA::MeasureUnit> measure_unit_entries; ///< Storage for extracted measure unit entries
};
