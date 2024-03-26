import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("data_report_tables/data_table_1.csv", header=None)
df2 = pd.read_csv("data_report_tables/data_table_3.csv", header=None)

n_samples = 100

print('-----------------------------------------------------')
num_batches = len(df) // n_samples
for i in range(num_batches):
    start_index = i * n_samples
    end_index = min((i + 1) * n_samples, len(df))
    batch = df.iloc[start_index:end_index]
    # Calculate average, standard deviation, and maximum of the values in the first column
    avg_time = batch.iloc[:, 1].mean()
    worst_time = batch.iloc[:, 1].max()
    std_dev = batch.iloc[:, 1].std()

    n_points = batch.iloc[0,0]
    print(f'n: {n_points}, avg_time: {avg_time}, worst_time: {worst_time}, std_dev: {std_dev}')

print('-----------------------------------------------------')

table_2_n_values = [10, 100, 200, 500, 1000]

x_1, x_2, x_3, x_4 = 1.1, 1.2, 1.5, 2

for i in range(num_batches):
    start_index = i * n_samples
    end_index = min((i + 1) * n_samples, len(df))
    batch = df.iloc[start_index:end_index]
    # Calculate average, standard deviation, and maximum of the values in the first column
    avg_time = batch.iloc[:, 1].mean()
    n_points = batch.iloc[0,0]

    if n_points not in table_2_n_values:
        continue

    mark_1, mark_2, mark_3, mark_4 = x_1*avg_time, x_2*avg_time, x_3*avg_time, x_4*avg_time
    cnt_1, cnt_2, cnt_3, cnt_4 = 0, 0, 0, 0

    runtimes = batch.iloc[:,1]
    for runtime in runtimes:
        if runtime>mark_1:
            cnt_1+=1
        if runtime>mark_2:
            cnt_2+=1
        if runtime>mark_3:
            cnt_3+=1
        if runtime>mark_4:
            cnt_4+=1

    print(f'n: {n_points}, avg_time: {avg_time}')
    print(f'Fraction of runtime samples exceeding the average by {round((x_1-1)*100)}%: {(cnt_1/n_samples)*100}, {round((x_2-1)*100)}%: {(cnt_2/n_samples)*100}, {round((x_3-1)*100)}%: {(cnt_3/n_samples)*100}, {round((x_4-1)*100)}%: {(cnt_4/n_samples)*100}')
print('-----------------------------------------------------')

print('-----------------------------------------------------')
n_samples = 25
num_batches = len(df2) // n_samples
for i in range(num_batches):
    start_index = i * n_samples
    end_index = min((i + 1) * n_samples, len(df2))
    batch = df2.iloc[start_index:end_index]
    # Calculate average, standard deviation, and maximum of the values in the first column

    avg_time_welzl = batch.iloc[:, 1].mean()
    avg_time_cgal = batch.iloc[:, 2].mean()
    avg_time_det = batch.iloc[:, 3].mean()

    n_points = batch.iloc[0,0]
    print(f'n: {n_points}, Average runtime for Welzl: {avg_time_welzl}, Cgal: {avg_time_cgal}, Deterministic: {avg_time_det}')

print('-----------------------------------------------------')

# print('------------------------------------------------\n')