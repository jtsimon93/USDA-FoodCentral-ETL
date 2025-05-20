#include "services/PipelineManager.h"
#include <future>
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

  // Launch parsing tasks concurrently
  auto food_entries_future = std::async(std::launch::async, [&]() {
    return food_extractor_service.GetFoodEntries();
  });
  auto food_category_entries_future = std::async(std::launch::async, [&]() {
    return food_category_extractor_service.GetFoodCategoryEntries();
  });
  auto nutrient_entries_future = std::async(std::launch::async, [&]() {
    return nutrient_extractor_service.GetNutrientEntries();
  });
  auto food_nutrient_entries_future = std::async(std::launch::async, [&]() {
    return food_nutrient_extractor_service.GetFoodNutrientEntries();
  });
  auto food_portion_entries_future = std::async(std::launch::async, [&]() {
    return food_portion_extractor_service.GetFoodPortionEntries();
  });
  auto measure_unit_entries_future = std::async(std::launch::async, [&]() {
    return measure_unit_extractor_service.GetMeasureUnitEntries();
  });
  auto branded_food_entries_future = std::async(std::launch::async, [&]() {
    return branded_food_extractor_service.GetBrandedFoodEntries();
  });

  // Block and wait for all tasks to finish
  // Ordered by least memory usage to most
  food_category_entries = std::move(food_category_entries_future.get());
  measure_unit_entries = std::move(measure_unit_entries_future.get());
  nutrient_entries = std::move(nutrient_entries_future.get());
  food_portion_entries = std::move(food_portion_entries_future.get());
  food_entries = std::move(food_entries_future.get());
  branded_food_entries = std::move(branded_food_entries_future.get());
  food_nutrient_entries = std::move(food_nutrient_entries_future.get());

  // Reporting
  std::cout << "Parsed " << food_entries.size() << " food entries:\n";
  std::cout << "Parsed " << food_category_entries.size()
            << " food category entries:\n";
  std::cout << "Parsed " << nutrient_entries.size() << " nutrient entries.\n";
  std::cout << "Parsed " << food_nutrient_entries.size()
            << " food nutrient entries.\n";
  std::cout << "Parsed " << food_portion_entries.size()
            << " food portion entries.\n";
  std::cout << "Parsed " << measure_unit_entries.size()
            << " measure unit entries.\n";
  std::cout << "Parsed " << branded_food_entries.size()
            << " branded food entries.\n";

  auto total_entries =
      food_entries.size() + food_category_entries.size() + nutrient_entries.size() +
      food_nutrient_entries.size() + food_portion_entries.size() +
      measure_unit_entries.size() + branded_food_entries.size();
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);

  std::cout << "Time taken to parse all entries (" << total_entries
            << "): " << duration.count() << " milliseconds.\n";

}
