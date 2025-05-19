#include "services/FoodCategoryExtractorService.h"
#include "csv-parser.h"

FoodCategoryExtractorService::FoodCategoryExtractorService(
    const std::string &food_category_input_file)
    : food_category_input_file(food_category_input_file) {}

std::vector<USDA::FoodCategory> &
FoodCategoryExtractorService::GetFoodCategoryEntries() {
  ExtractFoodCategoryEntries();
  return food_category_entries;
}

void FoodCategoryExtractorService::ExtractFoodCategoryEntries() {
  csv::CSVReader reader(food_category_input_file);

  for (csv::CSVRow &row : reader) {
    USDA::FoodCategory food_category;
    try {
      food_category.id = row[0].get<int>();
      food_category.code = row[1].get<int>();
      food_category.description = row[2].get<std::string>();

      food_category_entries.push_back(food_category);
    } catch (const std::exception &e) {
      std::cerr << "Failed to parse food category row: " << e.what() << "\n";
    }
  }
}
