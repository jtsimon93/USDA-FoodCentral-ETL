#pragma once

#include "services/BrandedFoodExtractorService.h"
#include "services/FoodCategoryExtractorService.h"
#include "services/FoodExtractorService.h"
#include "services/FoodNutrientExtractorService.h"
#include "services/FoodPortionExtractorService.h"
#include "services/MeasureUnitExtractorService.h"
#include "services/NutrientExtractorService.h"
#include <string>
#include <unordered_map>

class PipelineManager {
public:
  PipelineManager(
      const std::unordered_map<std::string, std::string> &input_map);
  void Run();

private:
  std::unordered_map<std::string, std::string> input_map;
  FoodExtractorService food_extractor_service;
  FoodCategoryExtractorService food_category_extractor_service;
  NutrientExtractorService nutrient_extractor_service;
  FoodNutrientExtractorService food_nutrient_extractor_service;
  FoodPortionExtractorService food_portion_extractor_service;
  MeasureUnitExtractorService measure_unit_extractor_service;
  BrandedFoodExtractorService branded_food_extractor_service;
};
