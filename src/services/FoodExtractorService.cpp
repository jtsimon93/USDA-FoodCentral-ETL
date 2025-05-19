#include "services/FoodExtractorService.h"
#include "csv-parser.h"

FoodExtractorService::FoodExtractorService(const std::string &food_input_file)
    : food_input_file(food_input_file) {}

std::vector<USDA::Food> &FoodExtractorService::GetFoodEntries() {
  ExtractFoodEntries();
  return food_entries;
}

void FoodExtractorService::ExtractFoodEntries() {
  csv::CSVReader reader(food_input_file);

  for (csv::CSVRow &row : reader) {
    USDA::Food food;
    try {
      food.fdc_id = row[0].get<int>();
      food.data_type = row[1].get<>();
      food.description = row[2].get<>();
      food.food_category_id = row[3].is_null() ? -9999 : row[3].get<int>();

      std::string date = row[4].get<>();
      if (date.size() >= 10) {
        food.publication_date = std::chrono::year_month_day(
            std::chrono::year(std::stoi(date.substr(0, 4))),
            std::chrono::month(std::stoi(date.substr(5, 2))),
            std::chrono::day(std::stoi(date.substr(8, 2))));
      }

      food_entries.push_back(food);
    } catch (const std::exception &e) {
      std::cerr << "Failed to parse food row: " << e.what() << "\n";
    }
  }
}
