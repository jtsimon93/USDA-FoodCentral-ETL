#pragma once

#include "models/usda/Nutrient.h"
#include <vector>

class NutrientExtractorService {
public:
  NutrientExtractorService(const std::string &nutrient_input_file);
  ~NutrientExtractorService() = default;

  std::vector<USDA::Nutrient> &GetNutrientEntries();

private:
  void ExtractNutrientEntries();

  std::string nutrient_input_file;
  std::vector<USDA::Nutrient> nutrient_entries;
};
