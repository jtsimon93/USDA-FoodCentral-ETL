#include "services/extractors/FoodPortionExtractorService.h"
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
      food_portion.seq_num = row[2].is_null()
                                 ? std::nullopt
                                 : std::make_optional(row[2].get<int>());
      food_portion.amount = row[3].is_null()
                                ? std::nullopt
                                : std::make_optional(row[3].get<float>());
      food_portion.measure_unit_id =
          row[4].is_null() ? std::nullopt
                           : std::make_optional(row[4].get<int>());

      // Optional fields
      food_portion.portion_description =
          std::make_optional(row[5].get<std::string>());
      food_portion.modifier =
          row[6].is_null() ? std::nullopt
                           : std::make_optional(row[6].get<std::string>());
      food_portion.gram_weight = row[7].is_null()
                                     ? std::nullopt
                                     : std::make_optional(row[7].get<float>());
      food_portion.data_points = row[8].is_null()
                                     ? std::nullopt
                                     : std::make_optional(row[8].get<int>());
      food_portion.footnote =
          row[9].is_null() ? std::nullopt
                           : std::make_optional(row[9].get<std::string>());
      food_portion.min_year_acquired =
          row[10].is_null() ? std::nullopt
                            : std::make_optional(row[10].get<int>());

      food_portion_entries.push_back(food_portion);
    } catch (const std::exception &e) {
      std::cerr << "Failed to parse food portion row: " << e.what() << "\n";
    }
  }
}
