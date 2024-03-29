import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import math
from sklearn.linear_model import LinearRegression

df = pd.read_csv("data_report_tables/data_table_4.csv", header=None)

n_samples = 100
num_batches = len(df) // n_samples
sec_runtimes = []
cgal_runtimes = []
xs = []
for i in range(num_batches):
    start_index = i * n_samples
    end_index = min((i + 1) * n_samples, len(df))
    batch = df.iloc[start_index:end_index]
    # Calculate average, standard deviation, and maximum of the values in the first column

    avg_time_sec = batch.iloc[:, 1].mean()
    avg_time_cgal = batch.iloc[:, 2].mean()

    n_points = batch.iloc[0,0]
    sec_runtimes.append(avg_time_sec)
    cgal_runtimes.append(avg_time_cgal)
    xs.append(n_points)


xs = np.array(xs).reshape(-1, 1)
sec_runtimes = np.array(sec_runtimes)
cgal_runtimes = np.array(cgal_runtimes)

regr_sec = LinearRegression()
regr_sec.fit(xs, sec_runtimes)
a_sec = regr_sec.coef_
b_sec = regr_sec.intercept_

regr_cgal = LinearRegression()
regr_cgal.fit(xs, cgal_runtimes)
a_cgal = regr_cgal.coef_
b_cgal = regr_cgal.intercept_

plt.scatter(xs, sec_runtimes, color='green', label='Expected O(n) algorithm (Algo 1)')
plt.scatter(xs, cgal_runtimes, color='yellow', label='CGAL in-built algorithm (Algo 2)')
plt.plot(xs, a_sec*xs + b_sec, color='red', label=f'Best fit for Algo 1 (slope = {a_sec})')
plt.plot(xs, a_cgal*xs + b_cgal, color='blue', label=f'Best fit for Algo 2 (slope = {a_cgal})')
plt.xlabel('Number of points')
plt.ylabel('Runtime (in ms)')
plt.title('Comparison of Randomized v/s CGAL in-built algorithm Runtimes')
plt.legend()
plt.savefig('plots/sec_vs_cgal.png')
