================ Prerequisites ================

Make sure the latest version of C++ (at least 8.xx) is installed.

Download the repository

lessons.json expected format
{
    "Lessons":{
        "1":{
            "name": "Greek",
            "classes":[
                    {"year": "A", "hours": 3},
                    {"year": "C", "hours": 2}
            ]
        }
}


teacher.json expected format
{
    "Teachers": {
        "1": {
            "name": "Papadopoulos",
            "teaches": ["1", "2", "3"],
            "hoursPerDay": 6,
            "hoursPerWeek": 20
        }
}


================ Compilation via visual studio ================

Visual Studio's compiler can significantly improve performance
Pick your hyperparameters at the top of main and simply run the file in release mode.


================ Εκτέλεση μέσω gcc ================

For this method you need to update the structure:

From the root folder, create a folder named nlohmann and place the json.hpp file inside it. You can download it from:
https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp
This is required to enable support for JSON files.

Create a folder named data, which should contain the files:

    lessons.json

    teacher.json

Create a folder named data_analysis, where the program will generate the file:

    evaluation_metrics.csv
    This file is used for creating visualizations.

Open a terminal or command prompt in the root directory (where all .cpp and .h files are located), and run:
Windows
g++ -O3 -std=c++17 -I. *.cpp
Linux
g++ -std=c++17 -I. *.cpp -ltbb


================ Additional Features  ================

Folder creation is optional. You can specify custom paths by modifying the following variables at the top of main.cpp 
LESSON_DATA_PATH
TEACHER_DATA_PATH
EVAL_METRICS_PATH

You can provide a custom seed by assigning an unsigned int to the seed variable at the top of main.cpp.

To speed up execution (at the cost of potentially less optimal results), modify:
POPULATION_SIZE
MAX_GEN

To enable detailed debug output (slower execution), uncomment the line:
    // #define DEBUG


================ Special thanks ================
Json.hpp (https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp) was used for json support.
