#include "services/extractors/MeasureUnitExtractorService.h"
#include "csv-parser.h"

MeasureUnitExtractorService::MeasureUnitExtractorService(
    const std::string &measure_unit_input_file)
    : measure_unit_input_file(measure_unit_input_file) {}

std::vector<USDA::MeasureUnit> &
MeasureUnitExtractorService::GetMeasureUnitEntries() {
  ExtractMeasureUnitEntries();
  return measure_unit_entries;
}

void MeasureUnitExtractorService::ExtractMeasureUnitEntries() {
  csv::CSVReader reader(measure_unit_input_file);

  for (csv::CSVRow &row : reader) {
    USDA::MeasureUnit measure_unit;
    try {
      measure_unit.id = row[0].get<int>();
      measure_unit.name = row[1].get<std::string>();

      measure_unit_entries.push_back(measure_unit);
    } catch (const std::exception &e) {
      std::cerr << "Failed to parse measure unit row: " << e.what() << "\n";
    }
  }
}
