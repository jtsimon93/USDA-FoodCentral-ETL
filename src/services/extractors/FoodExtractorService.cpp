#include "services/extractors/FoodExtractorService.h"
#include "csv-parser/csv-parser.h"
#include <optional>

FoodExtractorService::FoodExtractorService(const std::string &food_input_file)
    : food_input_file(food_input_file) {}

std::vector<USDA::Food> &FoodExtractorService::GetFoodEntries() {
  ExtractFoodEntries();
  return food_entries;
}

void FoodExtractorService::ExtractFoodEntries() {
  food_entries.reserve(2000000); // Pre-allocate for expected size

  csv::CSVReader reader(food_input_file);

  for (csv::CSVRow &row : reader) {
    USDA::Food food;
    try {
      // Filter by data type - only include foundation and branded foods
      // This is a key filtering step that determines which entries appear in
      // the master list
      const std::string data_type = row[1].get<std::string>();
      if (data_type != "foundation_food" && data_type != "branded_food") {
        continue;
      }
      food.fdc_id = row[0].get<int>();
      food.data_type = row[1].get<std::string>() == "foundation_food"
                           ? USDA::FoodDataType::Foundation
                           : USDA::FoodDataType::Branded;
      food.description = row[2].get<std::string>();
      food.food_category_id =
          row[3].is_null() ? std::nullopt
                           : std::make_optional(row[3].get<std::string>());

      // Parse publication date in ISO format (YYYY-MM-DD)
      std::string date = row[4].get<std::string>();
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

  // Optimize memory usage after loading is complete
  food_entries.shrink_to_fit();
}
