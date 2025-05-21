#include "services/extractors/NutrientExtractorService.h"
#include "csv-parser.h"

NutrientExtractorService::NutrientExtractorService(
    const std::string &nutrient_input_file)
    : nutrient_input_file(nutrient_input_file) {}

std::vector<USDA::Nutrient> &NutrientExtractorService::GetNutrientEntries() {
  ExtractNutrientEntries();
  return nutrient_entries;
}

void NutrientExtractorService::ExtractNutrientEntries() {
  nutrient_entries.reserve(500); // Pre-allocate for expected size

  csv::CSVReader reader(nutrient_input_file);

  for (csv::CSVRow &row : reader) {
    USDA::Nutrient nutrient;
    try {
      nutrient.id = row[0].get<int>();
      nutrient.name = row[1].get<std::string>();
      nutrient.unit_name = row[2].get<std::string>();
      nutrient.nutrient_nbr =
          row[3].is_null() ? std::nullopt
                           : std::make_optional(row[3].get<std::string>());
      nutrient.rank = row[4].is_null()
                          ? std::nullopt
                          : std::make_optional(row[4].get<float>());

      nutrient_entries.push_back(nutrient);
    } catch (const std::exception &e) {
      std::cerr << "Failed to parse nutrient row: " << e.what() << "\n";
    }
  }

  // Optimize memory usage after loading is complete
  nutrient_entries.shrink_to_fit();
}
