import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('bin/data_analysis/evaluation_metrics.csv')

# Convert the columns to numpy arrays (if needed)
best_score = df['BestScore'].to_numpy()
avg_score = df['AvgScore'].to_numpy()
generation = df['Generation'].to_numpy()

# Plot the data
plt.plot(generation, avg_score, label='Avg Score')
plt.plot(generation, best_score, label='Best Score')

plt.xlabel('Generation')
plt.ylabel('Score')
plt.title('Evaluation Metrics Over Generations')
plt.legend()
plt.show()

