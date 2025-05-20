#include "services/FoodNutrientExtractorService.h"
#include "csv-parser.h"

FoodNutrientExtractorService::FoodNutrientExtractorService(
    const std::string &food_nutrient_input_file)
    : food_nutrient_input_file(food_nutrient_input_file) {}

std::vector<USDA::FoodNutrient> &
FoodNutrientExtractorService::GetFoodNutrientEntries() {
  ExtractFoodNutrientEntries();
  return food_nutrient_entries;
}

void FoodNutrientExtractorService::ExtractFoodNutrientEntries() {
  csv::CSVReader reader(food_nutrient_input_file);

  for (csv::CSVRow &row : reader) {
    USDA::FoodNutrient food_nutrient;
    try {
      food_nutrient.id = row[0].get<int>();
      food_nutrient.fdc_id = row[1].get<int>();
      food_nutrient.nutrient_id = row[2].get<int>();
      food_nutrient.amount = row[3].is_null()
                                 ? std::nullopt
                                 : std::make_optional(row[3].get<float>());

      // Optional metadata
      food_nutrient.data_points = row[4].is_null()
                                      ? std::nullopt
                                      : std::make_optional(row[4].get<int>());
      food_nutrient.derivation_id =
          row[5].is_null() ? std::nullopt
                           : std::make_optional(row[5].get<std::string>());
      food_nutrient.min = row[6].is_null()
                              ? std::nullopt
                              : std::make_optional(row[6].get<float>());
      food_nutrient.max = row[7].is_null()
                              ? std::nullopt
                              : std::make_optional(row[7].get<float>());
      food_nutrient.median = row[8].is_null()
                                 ? std::nullopt
                                 : std::make_optional(row[8].get<float>());
      food_nutrient.loq = row[9].is_null()
                              ? std::nullopt
                              : std::make_optional(row[9].get<float>());
      food_nutrient.footnote =
          row[10].is_null() ? std::nullopt
                            : std::make_optional(row[10].get<std::string>());
      food_nutrient.min_year_acquired =
          row[11].is_null() ? std::nullopt
                            : std::make_optional(row[11].get<int>());
      food_nutrient.percent_daily_value =
          row[12].is_null() ? std::nullopt
                            : std::make_optional(row[12].get<float>());

      food_nutrient_entries.push_back(food_nutrient);
    } catch (const std::exception &e) {
      std::cerr << "Failed to parse food nutrient row: " << e.what() << "\n";
    }
  }
}
