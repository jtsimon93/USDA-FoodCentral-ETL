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

class PipelineManager {
public:
  PipelineManager(
      const std::unordered_map<std::string, std::string> &input_map);
  void ProcessData();

private:
  void ExtractData();
  void TransformData();
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
