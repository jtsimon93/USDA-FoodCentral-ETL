#pragma once

#include "models/usda/FoodPortion.h"
#include <string>
#include <vector>

class FoodPortionExtractorService {
public:
  FoodPortionExtractorService(const std::string &food_portion_input_file);
  ~FoodPortionExtractorService() = default;

  std::vector<USDA::FoodPortion> &GetFoodPortionEntries();

private:
  void ExtractFoodPortionEntries();

  std::string food_portion_input_file;
  std::vector<USDA::FoodPortion> food_portion_entries;
};
