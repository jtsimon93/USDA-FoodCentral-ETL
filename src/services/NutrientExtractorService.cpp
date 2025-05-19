#include "services/NutrientExtractorService.h"
#include "csv-parser.h"

NutrientExtractorService::NutrientExtractorService(
    const std::string &nutrient_input_file)
    : nutrient_input_file(nutrient_input_file) {}

std::vector<USDA::Nutrient> &NutrientExtractorService::GetNutrientEntries() {
  ExtractNutrientEntries();
  return nutrient_entries;
}

void NutrientExtractorService::ExtractNutrientEntries() {
    csv::CSVReader reader(nutrient_input_file);

    for(csv::CSVRow &row : reader) {
        USDA::Nutrient nutrient;
        try {
            nutrient.id = row[0].get<int>();
            nutrient.name = row[1].get<std::string>();
            nutrient.unit_name = row[2].get<std::string>();
            nutrient.nutrient_nbr = row[3].is_null() ? -9999 : row[3].get<float>();
            nutrient.rank = row[4].is_null() ? -9999 : row[4].get<float>();

            nutrient_entries.push_back(nutrient);
        } catch(const std::exception &e) {
            std::cerr << "Failed to parse nutrient row: " << e.what() << "\n";
        }
    }
}
