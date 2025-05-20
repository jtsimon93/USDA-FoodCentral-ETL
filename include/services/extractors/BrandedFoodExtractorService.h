#pragma once

#include "models/usda/BrandedFood.h"
#include <string>
#include <vector>

class BrandedFoodExtractorService {

public:
  BrandedFoodExtractorService(const std::string &branded_food_input_file);
  ~BrandedFoodExtractorService() = default;

  std::vector<USDA::BrandedFood> &GetBrandedFoodEntries();

private:
  void ExtractBrandedFoodEntries();

  std::string branded_food_input_file;
  std::vector<USDA::BrandedFood> branded_food_entries;
};
