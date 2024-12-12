import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the CSV file
df = pd.read_csv('bin/data_analysis/evaluation_metrics.csv')

# Convert the columns to numpy arrays (if needed)
best_score = df['BestScore'].to_numpy()
avg_score = df['AvgScore'].to_numpy()
generation = df['Generation'].to_numpy()

# Plot the data
plt.plot(generation, avg_score, label='Avg Score')
plt.plot(generation, best_score, label='Best Score')

reference_value = 3*3*19 # total lessons * classesPerGrade * grades
#reference_value = 3*6 #simpleLessons
max_best_score = np.max(best_score)

# Calculate the percentage of each maximum score compared to the reference value
percentage_best = (max_best_score**(1/6) / reference_value) * 100

# Output the comparison
print(f"Maximum Best Score: {max_best_score}")
print(f"Percentage of resulted to potential best for calculateSatisfyLessonHoursScore: {percentage_best:.2f}%")

plt.xlabel('Generation')
plt.ylabel('Score')
plt.title('Evaluation Metrics Over Generations')
plt.legend()
plt.show()


