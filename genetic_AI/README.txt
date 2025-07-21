================ Προϋποθέσεις ================

Πρέπει να είναι εγκατεστημένη η τελευταία έκδοση (τουλάχιστον 8.xx).

Από τον root folder, δημιουργήστε έναν φάκελο και ονομάστε τον nlohmann στον οποίο θα τοποθετήσετε το json.hpp (https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp) για να υποστηρίζονται json files.

Δημιουργήστε έναν φάκελο data στον οποίο θα είναι τοποθετημένα τα αρχεία lessons.json και teacher.json.

Δημιουργήστε έναν φάκελο data_analysis στον οποίο θα δημιουργείται το αρχείο evaluation_metrics.csv για την δημιουργία γραφικών.

Το lessons.json πρέπει να είναι της μορφής
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


Το teacher.json πρέπει να είναι της μορφής
{
    "Teachers": {
        "1": {
            "name": "Papadopoulos",
            "teaches": ["1", "2", "3"],
            "hoursPerDay": 6,
            "hoursPerWeek": 20
        }
}

================ Εκτέλεση μέσω gcc ================

Αρκεί να εκτελέσετε στο cmd την παρακάτω εντολή στο root, στο οποίο είναι όλα τα .cpp και .h files
Windows
g++ -O3 -std=c++17 -I. *.cpp
Linux
g++ -std=c++17 -I. *.cpp -ltbb

================ Επιπλέον δυνατότητες ================
Η δημιουργία φακέλων δεν είναι υποχρεωτική, μπορείτε να δώσετε το δικό σας κατάλληλο path μέσω των μεταβλητών 
LESSON_DATA_PATH
TEACHER_DATA_PATH
EVAL_METRICS_PATH
οι οποίες βρίσκονται στην αρχή του main.cpp

Μπορείτε να δώσετε το δικό σας seed, δίνοντας ένα unsigned int στην μεταβλητή seed στην αρχή του main.cpp

Επιπλέον μπορείτε να αλλάξετε τις μεταβλητές
POPULATION_SIZE
MAX_GEN
για ταχύτερη εκτέλεση, χωρίς την επιβεβαίωση δημιουργίας optimal solution.

Αφαίρεση του comment // #define DEBUG στην κορυφή του main.cpp για αναλυτικότερο output, αλλά αργότερη εκτέλεση

================ Εκτέλεση μέσω visual studio ================
Ο compiler του visual studio μπορεί να επιταχύνει ραγδαία την εκτέλεση του προγράμματος
Δημιουργήστε ένα project με cpp version 17.
Τοποθετήστε τους φακέλους data και data_analysis σε έναν φάκελο bin στο root
Τοποθετήστε τον φάκελο nlohmann σε έναν φάκελο include
Αφού προσθέσετε τα hpp και cpp αρχεία, βεβαιωθείτε ότι ισχύουν οι παρακάτω ρυθμίσεις στα project properties (alt + enter)

General>OutputDirectory: $(SolutionDir)bin
General>C++ Language Standard: ISO C++17 Standard (/std:c++17)
Debugging>Working Directory: $(SolutionDir)bin
C/C++>General>Additional Include Directories: $(SolutionDir)include

Αφού σιγουρευτείτε ότι είστε σε Release πιέστε ctrl + shift + B για να κάνετε compile
Τέλος αρκεί η εκτέλεση του αρχείου .exe από το bin