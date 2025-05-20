#pragma once

#include "models/usda/FoodCategory.h"
#include <vector>

class FoodCategoryExtractorService {
public:
  FoodCategoryExtractorService(const std::string &food_category_input_file);
  ~FoodCategoryExtractorService() = default;

  std::vector<USDA::FoodCategory> &GetFoodCategoryEntries();

private:
  void ExtractFoodCategoryEntries();

  std::string food_category_input_file;
  std::vector<USDA::FoodCategory> food_category_entries;
};
