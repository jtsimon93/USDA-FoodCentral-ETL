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
      food_nutrient.amount = row[3].is_null() ? -9999 : row[3].get<float>();
      food_nutrient_entries.push_back(food_nutrient);
    } catch (const std::exception &e) {
      std::cerr << "Failed to parse food nutrient row: " << e.what() << "\n";
    }
  }
}
