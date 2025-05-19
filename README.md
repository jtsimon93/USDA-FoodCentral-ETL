# USDA FoodCentral Database ETL

This project is designed to model data from the USDA FoodCentral Database CSV files, with the goal of streamlining and accelerating ETL (Extract, Transform, Load) processes.

## Project Status

**Work in Progress:** Currently, the focus is on mapping essential columns, and this is in progress. Full column coverage is planned, but some fields are either unclear or may not be relevant. The primary focus at this stage is performance—exploring how efficiently the code can be executed.

Contributions are welcome. Feel free to use, adapt, or improve this project as needed.

## Building & Using the Project

You will need to specify the location of the input files in the `input_locations.txt` file. The project is designed to be flexible and adaptable to various input formats and sizes.

To build the project, use CMake. A CMakeLists.txt file is included in the repository for your convenience.

## Acknowledgements

This project utilizes Vincent La's CSV Parser: https://github.com/vincentlaucsb/csv-parser
