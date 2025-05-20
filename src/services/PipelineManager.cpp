#include "services/PipelineManager.h"
#include "services/BrandedFoodExtractorService.h"
#include "services/FoodNutrientExtractorService.h"
#include "services/NutrientExtractorService.h"
#include <iostream>

PipelineManager::PipelineManager(
    const std::unordered_map<std::string, std::string> &input_map) try
    : input_map(input_map),
      branded_food_extractor_service(input_map.at("branded_food_input_file")),
      food_category_extractor_service(input_map.at("food_category_input_file")),
      food_extractor_service(input_map.at("food_input_file")),
      food_nutrient_extractor_service(input_map.at("food_nutrient_input_file")),
      food_portion_extractor_service(input_map.at("food_portion_input_file")),
      measure_unit_extractor_service(input_map.at("measure_unit_input_file")),
      nutrient_extractor_service(input_map.at("nutrient_input_file")) {
} catch (const std::out_of_range &e) {
  std::cerr << "Missing key in input map: " << e.what() << std::endl;
  std::cerr << "Available keys: ";
  for (const auto &pair : input_map) {
    std::cerr << pair.first << " ";
  }
  std::cerr << std::endl;
  throw; // Rethrow to stop execution
}

void PipelineManager::Run() {
  auto start_time = std::chrono::high_resolution_clock::now();

  const auto &food_entries = food_extractor_service.GetFoodEntries();
  const auto &category_entries =
      food_category_extractor_service.GetFoodCategoryEntries();
  const auto &nutrients = nutrient_extractor_service.GetNutrientEntries();
  const auto &food_nutrients =
      food_nutrient_extractor_service.GetFoodNutrientEntries();
  const auto &portions = food_portion_extractor_service.GetFoodPortionEntries();
  const auto &units = measure_unit_extractor_service.GetMeasureUnitEntries();
  const auto &branded = branded_food_extractor_service.GetBrandedFoodEntries();

  // Reporting
  std::cout << "Parsed " << food_entries.size() << " food entries:\n";
  std::cout << "Parsed " << category_entries.size()
            << " food category entries:\n";
  std::cout << "Parsed " << nutrients.size() << " nutrient entries.\n";
  std::cout << "Parsed " << food_nutrients.size()
            << " food nutrient entries.\n";
  std::cout << "Parsed " << portions.size() << " food portion entries.\n";
  std::cout << "Parsed " << units.size() << " measure unit entries.\n";
  std::cout << "Parsed " << branded.size() << " branded food entries.\n";

  auto total_entries = food_entries.size() + category_entries.size() +
                       nutrients.size() + food_nutrients.size() +
                       portions.size() + units.size() + branded.size();
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);

  std::cout << "Time taken to parse all entries (" << total_entries
            << "): " << duration.count() << " milliseconds.\n";
}
