# USDA FoodData Central ETL

This project provides a high-performance pipeline for parsing and transforming data from the **USDA FoodData Central** CSV exports. It is intended to simplify and accelerate ETL (Extract, Transform, Load) workflows, particularly for use in real-world applications like fitness tracking or dietary analysis.

---

## 🎯 Project Goals

The primary goal is to parse the full USDA FoodData Central CSV export and produce a **clean, normalized dataset** suitable for use in apps or analysis pipelines.

This project is **opinionated**:

- It **excludes sample foods** and **sub-sample foods** due to inconsistency and poor data quality.
- It focuses only on **core, well-structured food types** such as `foundation_food` and `branded_food`.

---

## ✅ Current Features

- Efficient parsing of:
  - `food.csv`
  - `branded_food.csv`
  - `food_category.csv`
  - `food_nutrient.csv`
  - `food_portion.csv`
  - `nutrient.csv`
  - `measure_unit.csv`
- Excludes sample/subsample food records
- Optional field handling via `std::optional`
- Fast processing of **millions of records** in under 2 minutes

---

## 🚧 Project Status

> **Work in Progress**

Current development is focused on performance, data quality, and clean structuring. Full database export functionality is planned.

### Progress Tracker

- [x] Parse all essential tables and columns
- [x] Exclude `sample_food` and `sub_sample_food` entries
- [x] Handle optional/missing fields safely
- [ ] Explore other USDA CSVs for possible inclusion
- [ ] Normalize and clean key values (units, formats, etc.)
- [ ] Remove nutrient/portion entries linked to excluded foods
- [ ] Add export logic (to SQL files, DB connection, or CSVs)

---

## 🏗️ Build Instructions

### Prerequisites

- C++20 compiler (e.g. GCC 11+, Clang 13+)
- CMake ≥ 3.15
- `input_locations.txt` (see below)

### 🔨 To Build:

```bash
cmake -S . -B build
cmake --build build
```

### 📥 Input Files

Specify the location of your USDA CSV files in a file named:

```text
input_locations.txt
```

Example format:

```
food=path/to/food.csv
branded_food=path/to/branded_food.csv
...
```

This file is copied into the build directory during the build.

---

## 🧰 Use Cases

- Feeding cleaned food/nutrition data into a fitness or meal-tracking app
- Creating a simplified subset of USDA data for analytics
- Quickly inspecting and filtering high-volume USDA exports

---

## 📚 Acknowledgements

This project uses the excellent [CSV Parser by Vincent La](https://github.com/vincentlaucsb/csv-parser) for efficient and easy CSV handling.

---

## 🤝 Contributions

Contributions, bug reports, and pull requests are welcome! This is an open project aimed at helping others integrate USDA food data into their tools.

---

## 📜 License

This project is released under the MIT License.
