#include "services/FoodPortionExtractorService.h"
#include "csv-parser.h"

FoodPortionExtractorService::FoodPortionExtractorService(
    const std::string &food_portion_input_file)
    : food_portion_input_file(food_portion_input_file) {}

std::vector<USDA::FoodPortion> &
FoodPortionExtractorService::GetFoodPortionEntries() {
  ExtractFoodPortionEntries();
  return food_portion_entries;
}

void FoodPortionExtractorService::ExtractFoodPortionEntries() {
  csv::CSVReader reader(food_portion_input_file);

  for (csv::CSVRow &row : reader) {
    USDA::FoodPortion food_portion;
    try {
      food_portion.id = row[0].get<int>();
      food_portion.fdc_id = row[1].get<int>();
      food_portion.seq_num = row[2].is_null() ? -9999 : row[2].get<int>();
      food_portion.amount = row[3].is_null() ? -9999 : row[3].get<float>();
      food_portion.measure_unit_id =
          row[4].is_null() ? -9999 : row[4].get<int>();
      food_portion.portion_description = row[5].get<std::string>();
      food_portion.modifier = row[6].is_null() ? "" : row[6].get<std::string>();
      food_portion.gram_weight = row[7].is_null() ? -9999 : row[7].get<float>();

      food_portion_entries.push_back(food_portion);
    } catch (const std::exception &e) {
      std::cerr << "Failed to parse food portion row: " << e.what() << "\n";
    }
  }
}
