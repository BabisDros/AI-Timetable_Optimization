# Artificial intelligence project
The aim of this project was to gain familiarity with basic **artificial intelligence algorithms** and **propositional logic**, presented as two independent sub-projects. It was developed during the 5th semester as part of the Artificial Intelligence course at AUEB.


## Tasks

We were tasked with implementing the following:

* A [genetic algorithm](genetic_AI) that given a set of parameters (how many times each subjects needs to be taught, how many hours a specific teacher can work, etc.) will create the ideal curriculum:

* 2 inference methods:
  * **Forward chaining** for Horn clauses (more precisely, definite clauses) in [propositional logic](propositional_Logic\ForwardChaining_Propositional), assuming a goal formula consisting of a single, non-negated propositional symbol (e.g., P).
  * **Forward chaining** for Horn clauses in [first-order predicate logic](propositional_Logic\ForwardChaining_FirstOrder), assuming a goal formula that is a non-negated atomic predicate (e.g., Loves(John, x)).

## Architecture
Our [genetic algorithm](genetic_AI) was developed in C++ as we aimed for better *multithreaded* runtime efficiency. [Chromosome](genetic_AI\AI-Timetable_Optimization\chromosome.h) is keeping track of a possible school curriculum. [Score](genetic_AI\AI-Timetable_Optimization\score.cpp) is used to evaluate a chromosome's fitness score. [Main](genetic_AI\AI-Timetable_Optimization\main.cpp) loads the [lessons](genetic_AI\bin\data\lessons.json)' and [teachers](genetic_AI\bin\data\teachers.json)' data, evaluates the population, breeds and mutates till we reach the generation's limit.

[PL_FC_Entails](propositional_Logic\ForwardChaining_Propositional\PL_FC_Entails.java) initialized a queue with known facts and it iteratively infers new facts until the query Q is derived or the queue is exhausted, based on the knowledge given by [horn clauses](propositional_Logic\ForwardChaining_Propositional\horn_clauses.txt).

[FOL](propositional_Logic\ForwardChaining_FirstOrder\FOL.java) Iteratively applies rules from the [KB](propositional_Logic\ForwardChaining_FirstOrder\KB_FOL.txt) to generate new facts, after it parses them to a acceptable Horn clauses.


## Statistics of genetic algorithm
Using [simpleLessons](genetic_AI\bin\data\simpleLessons.json) and [simpleTeachers](genetic_AI\bin\data\simpleTeachers.json), we were able to find a usable curriculum, but we only allowed calculateSatisfyLessonHoursScore (each lesson must appear x,y,z times for all classes) for our evaluation
<pre>
Best Fitness Achieved: 100%
Population: 20.000
Generations: 2.500
Starting Mutation Rate: 0.2
Mutation Decrease: 20%
Seed: 1426592087.
</pre>
<img src="images\perfect-cur-single.jpg"/>

If we enable all the possible evaluations, we reached a 70% acceptance rate with our parameters (mutation rate updates every 50 generations).
<pre>
Best Fitness Achieved: 70.4%
Population: 20.000
Generations: 5.000
Αρχικό Mutation Rate: 0.1
Minimum Mutation Rate: 0.0001
ADJUSTMENT_FACTOR: 1.2
</pre>
<img src="images\cur-all.jpg"/>

A higher score *can* be achieved by increasing the population size and the number of generations.

## How to run
Refer to our guides for the [genetic algorithm](genetic_AI\README.txt) and the [propositional logic](propositional_Logic\README.txt)

## Contributors
<a href="https://github.com/BabisDros/AI-Timetable_Optimization/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=BabisDros/AI-Timetable_Optimization"/>
</a>
