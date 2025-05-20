#include "services/FoodExtractorService.h"
#include "csv-parser.h"
#include <optional>

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
      // We are only working with foundation and branded foods. Skip anything
      // else.
        if (row[1].get<std::string>() != "foundation_food" &&
            row[1].get<std::string>() != "branded_food") {
            continue;
        }
      food.fdc_id = row[0].get<int>();
      food.data_type = row[1].get<std::string>() == "foundation_food" ? USDA::FoodDataType::Foundation : USDA::FoodDataType::Branded;
      food.description = row[2].get<std::string>();
      food.food_category_id = row[3].is_null() ? std::nullopt : row[3].get<std::optional<int>>();

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
