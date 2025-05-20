#pragma once
#include "models/usda/Food.h"

class FoodExtractorService {
public:
  FoodExtractorService(const std::string &food_input_file);
  ~FoodExtractorService() = default;

  std::vector<USDA::Food> &GetFoodEntries();

private:
  void ExtractFoodEntries();

  std::string food_input_file;
  std::vector<USDA::Food> food_entries;
};
