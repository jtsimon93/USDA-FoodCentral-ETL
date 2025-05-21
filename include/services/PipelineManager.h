#pragma once

#include "models/usda/BrandedFood.h"
#include "services/extractors/BrandedFoodExtractorService.h"
#include "services/extractors/FoodCategoryExtractorService.h"
#include "services/extractors/FoodExtractorService.h"
#include "services/extractors/FoodNutrientExtractorService.h"
#include "services/extractors/FoodPortionExtractorService.h"
#include "services/extractors/MeasureUnitExtractorService.h"
#include "services/extractors/NutrientExtractorService.h"
#include <string>
#include <unordered_map>

/**
 * @class PipelineManager
 * @brief Manages the ETL (Extract, Transform, Load) pipeline for USDA Food Central data.
 *
 * This class coordinates the complete data processing workflow:
 * 1. Extraction of raw data from CSV files using specialized extractor services
 * 2. Transformation of the data to ensure integrity and consistency
 * 3. Future: Loading of processed data into target destinations
 *
 * The PipelineManager handles concurrent extraction of data using async tasks
 * and manages the memory-efficient processing of large USDA food datasets.
 */
class PipelineManager {
public:
  /**
   * @brief Constructs a PipelineManager with file path information.
   *
   * @param input_map Map of data type identifiers to file paths
   *                  Required keys:
   *                  - "food_input_file"
   *                  - "food_category_input_file"
   *                  - "nutrient_input_file"
   *                  - "food_nutrient_input_file"
   *                  - "food_portion_input_file"
   *                  - "measure_unit_input_file"
   *                  - "branded_food_input_file"
   * @throws std::out_of_range If any required key is missing from input_map
   */
  PipelineManager(
      const std::unordered_map<std::string, std::string> &input_map);

  /**
   * @brief Executes the complete ETL pipeline.
   *
   * Orchestrates the extraction and transformation of all USDA food data.
   * Processes are executed in sequence, with progress reporting to stdout.
   */
  void ProcessData();

private:
  /**
   * @brief Extracts data from all input files concurrently.
   *
   * Launches multiple async tasks to parse different data types in parallel,
   * optimizing for performance while managing memory usage. Results are moved
   * into member vectors with reporting of extraction statistics.
   */
  void ExtractData();

  /**
   * @brief Applies transformation operations to ensure data integrity.
   *
   * Executes a series of transformers to clean and validate the extracted data.
   * Current transformations:
   * - Removing entries with invalid FDC ID references
   */
  void TransformData();

  /**
   * @brief Loads the data into the database.
   */
  void LoadData();

  std::unordered_map<std::string, std::string> input_map;
  FoodExtractorService food_extractor_service;
  FoodCategoryExtractorService food_category_extractor_service;
  NutrientExtractorService nutrient_extractor_service;
  FoodNutrientExtractorService food_nutrient_extractor_service;
  FoodPortionExtractorService food_portion_extractor_service;
  MeasureUnitExtractorService measure_unit_extractor_service;
  BrandedFoodExtractorService branded_food_extractor_service;

  std::vector<USDA::FoodCategory> food_category_entries;
  std::vector<USDA::MeasureUnit> measure_unit_entries;
  std::vector<USDA::Nutrient> nutrient_entries;
  std::vector<USDA::FoodPortion> food_portion_entries;
  std::vector<USDA::Food> food_entries;
  std::vector<USDA::BrandedFood> branded_food_entries;
  std::vector<USDA::FoodNutrient> food_nutrient_entries;
};
