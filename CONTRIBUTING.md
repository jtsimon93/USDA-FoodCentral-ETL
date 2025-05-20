# Contributing to USDA FoodData Central ETL

First off, thanks for taking the time to contribute! 🎉

This project aims to create a clean, high-performance ETL tool for working with the USDA FoodData Central CSV datasets. Whether you're fixing bugs, adding new features, or improving documentation, your contributions are welcome.

---

## 🛠️ Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/jtsimon93/USDA-FoodCentral-ETL.git
cd USDA-FoodCentral-ETL
```

### 2. Build the Project

Ensure you have a C++20-capable compiler and CMake ≥ 3.15:

```bash
cmake -S . -B build
cmake --build build
```

### 3. Run It

Make sure `input_locations.txt` is set up with paths to the USDA CSV files before running.

---

## 🧪 Testing Your Changes

This project is currently light on formal tests. The best way to test is to:
- Run the ETL executable against your dataset
- Verify parsed row counts and data quality
- Check logs for exceptions or malformed data

You can run:

```bash
./build/USDA-FoodCentral-ETL
```

And compare output counts or run a debugger for step-through validation.

---

## 📁 Directory Structure

- `src/` — All implementation source files
- `include/` — Structs and service interfaces
- `input_locations.txt` — Required file for input CSV paths
- `build/` — CMake build directory

---

## 🧼 Code Style

- Use modern C++ (C++20 features welcome)
- Favor `std::optional`, `std::vector`, and RAII patterns
- Prefer clear, readable code over clever hacks
- Catch and log all exceptions during CSV parsing

---

## 🤝 How to Contribute

1. Fork the repo and create your branch
2. Make your changes
3. Commit and push
4. Submit a Pull Request and describe what it does

---

## 💬 Questions?

If you’re unsure how to proceed or want to discuss an idea before writing code, feel free to open a GitHub Issue or start a Discussion.

Thanks again!
