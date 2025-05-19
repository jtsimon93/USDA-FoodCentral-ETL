#pragma once

#include "models/usda/MeasureUnit.h"
#include <string>
#include <vector>

class MeasureUnitExtractorService {
public:
  MeasureUnitExtractorService(const std::string &measure_unit_input_file);
  ~MeasureUnitExtractorService() = default;

  std::vector<USDA::MeasureUnit> &GetMeasureUnitEntries();

private:
  void ExtractMeasureUnitEntries();

  std::string measure_unit_input_file;
  std::vector<USDA::MeasureUnit> measure_unit_entries;
};
