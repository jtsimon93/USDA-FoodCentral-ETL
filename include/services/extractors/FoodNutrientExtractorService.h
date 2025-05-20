#pragma once

#include "models/usda/FoodNutrient.h"
#include <string>
#include <vector>

class FoodNutrientExtractorService {
public:
  FoodNutrientExtractorService(const std::string &food_nutrient_input_file);
  ~FoodNutrientExtractorService() = default;

  std::vector<USDA::FoodNutrient> &GetFoodNutrientEntries();

private:
  void ExtractFoodNutrientEntries();

  std::string food_nutrient_input_file;
  std::vector<USDA::FoodNutrient> food_nutrient_entries;
};
