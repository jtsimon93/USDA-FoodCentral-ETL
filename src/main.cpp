#include "services/FoodCategoryExtractorService.h"
#include "services/FoodExtractorService.h"
#include "services/FoodNutrientExtractorService.h"
#include "services/FoodPortionExtractorService.h"
#include "services/MeasureUnitExtractorService.h"
#include "services/NutrientExtractorService.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

int main() {
  constexpr const char *input_locations_file_path = "input_locations.txt";
  std::unordered_map<std::string, std::string> input_map;

  std::ifstream input_file(input_locations_file_path);
  if (!input_file) {
    std::cerr << "Error opening file: " << input_locations_file_path
              << std::endl;
    return 1;
  }

  std::string line;
  while (std::getline(input_file, line)) {
    if (line.empty() || line[0] == '#')
      continue;

    std::istringstream iss(line);
    std::string key, value;

    if (std::getline(iss, key, '=') && std::getline(iss, value)) {
      input_map[key] = value;
    }
  }

  FoodExtractorService food_extractor_service(input_map["food_input_file"]);
  FoodCategoryExtractorService food_category_extractor_service(
      input_map["food_category_input_file"]);
  NutrientExtractorService nutrient_extractor_service(
      input_map["nutrient_input_file"]);
  FoodNutrientExtractorService food_nutrient_extractor_service(
      input_map["food_nutrient_input_file"]);
  FoodPortionExtractorService food_portion_extractor_service(
      input_map["food_portion_input_file"]);
  MeasureUnitExtractorService measure_unit_extractor_service(
      input_map["measure_unit_input_file"]);

  auto start_time = std::chrono::high_resolution_clock::now();

  const auto &food_entries = food_extractor_service.GetFoodEntries();
  const auto &food_category_entries =
      food_category_extractor_service.GetFoodCategoryEntries();
  const auto &nutrients = nutrient_extractor_service.GetNutrientEntries();
  const auto &food_nutrient_entries =
      food_nutrient_extractor_service.GetFoodNutrientEntries();
  const auto &food_portion_entries =
      food_portion_extractor_service.GetFoodPortionEntries();
  const auto &measure_unit_entries =
      measure_unit_extractor_service.GetMeasureUnitEntries();


  std::cout << "Parsed " << food_entries.size() << " food entries:\n";
  std::cout << "Parsed " << food_category_entries.size()
            << " food category entries:\n";
  std::cout << "Parsed " << nutrients.size() << " nutrient entries.\n";
  std::cout << "Parsed " << food_nutrient_entries.size()
            << " food nutrient entries.\n";
  std::cout << "Parsed " << food_portion_entries.size()
            << " food portion entries.\n";
  std::cout << "Parsed " << measure_unit_entries.size()
            << " measure unit entries.\n";

  auto end_time = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);

  std::cout << "Time taken to parse all entries: " << duration.count()
            << " milliseconds.\n";

  return 0;
}
