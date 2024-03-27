import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import math

df = pd.read_csv("data_report_tables/data_table_sec.csv", header=None)
df2 = pd.read_csv("data_report_tables/data_table_3.csv", header=None)

n_samples = 1000

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

table_2_n_values = [10, 100, 200, 500, 1000, 2000]

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


# Following code plots the number of samples (out of 1000) as a function of how much it exceeds the average
max_exceedance = 400
freqs_500, freqs_1000, freqs_2000, freqs_5000, freqs_10000, freqs_50000 = [0]*max_exceedance, [0]*max_exceedance, [0]*max_exceedance, [0]*max_exceedance, [0]*max_exceedance, [0]*max_exceedance
n_samples = 1000
required_n = [500, 1000, 2000, 5000, 10000, 50000]
for i in range(num_batches):
    start_index = i * n_samples
    end_index = min((i + 1) * n_samples, len(df))
    batch = df.iloc[start_index:end_index]
    # Calculate average, standard deviation, and maximum of the values in the first column
    avg_time = batch.iloc[:, 1].mean()
    n_points = batch.iloc[0,0]

    if n_points not in required_n:
        continue

    runtimes = batch.iloc[:,1]
    for runtime in runtimes:
        if runtime<avg_time:
            continue
        # print(f'runtime: {runtime}, avgtime: {avg_time}, fraction: {((runtime - avg_time)/avg_time)}')
        rem = math.floor(((runtime - avg_time)/avg_time)*100)
        if n_points == 500:
            freqs_500[rem]+=1
        if n_points == 1000:
            freqs_1000[rem]+=1
        if n_points == 2000:
            freqs_2000[rem]+=1
        if n_points == 5000:
            freqs_5000[rem]+=1
        if n_points == 10000:
            freqs_10000[rem]+=1
        if n_points == 50000:
            freqs_50000[rem]+=1

    print(f'n: {n_points}, avg_time: {avg_time}')
    print(f'Fraction of runtime samples exceeding the average by {round((x_1-1)*100)}%: {(cnt_1/n_samples)*100}, {round((x_2-1)*100)}%: {(cnt_2/n_samples)*100}, {round((x_3-1)*100)}%: {(cnt_3/n_samples)*100}, {round((x_4-1)*100)}%: {(cnt_4/n_samples)*100}')
print('-----------------------------------------------------')


freqs_500 = np.array(freqs_500)
for i in range(len(freqs_500)-2, -1, -1):
    freqs_500[i] += freqs_500[i+1]


freqs_1000 = np.array(freqs_1000)
for i in range(len(freqs_1000)-2, -1, -1):
    freqs_1000[i] += freqs_1000[i+1]

freqs_2000 = np.array(freqs_2000)
for i in range(len(freqs_2000)-2, -1, -1):
    freqs_2000[i] += freqs_2000[i+1]

freqs_5000 = np.array(freqs_5000)
for i in range(len(freqs_5000)-2, -1, -1):
    freqs_5000[i] += freqs_5000[i+1]

freqs_10000 = np.array(freqs_10000)
for i in range(len(freqs_10000)-2, -1, -1):
    freqs_10000[i] += freqs_10000[i+1]

freqs_50000 = np.array(freqs_50000)
for i in range(len(freqs_50000)-2, -1, -1):
    freqs_50000[i] += freqs_50000[i+1]

freqs_500 = (freqs_500/n_samples)*100
freqs_1000 = (freqs_1000/n_samples)*100
freqs_2000 = (freqs_2000/n_samples)*100
freqs_5000 = (freqs_5000/n_samples)*100
freqs_10000 = (freqs_10000/n_samples)*100
freqs_50000 = (freqs_50000/n_samples)*100

# plt.plot(np.arange(1, max_exceedance+1), np.array(freqs_500), color='r', label='N = 500')
# plt.plot(np.arange(1, max_exceedance+1), np.array(freqs_1000), color='b', label="N = 1000")
# plt.plot(np.arange(1, max_exceedance+1), np.array(freqs_2000), color='g', label="N = 2000")
# plt.plot(np.arange(1, max_exceedance+1), np.array(freqs_5000), color='yellow', label='N = 5000')
# plt.plot(np.arange(1, max_exceedance+1), np.array(freqs_10000), color='black', label="N = 10000")
plt.plot(np.arange(1, max_exceedance+1), np.array(freqs_50000), color='grey', label="N = 50000")
plt.xlabel('Average exceeded by (%)')
plt.ylabel('Percentage of all the samples')
plt.title('Percentage of samples exceeding the average runtime by a certain amount')
plt.legend()
plt.savefig('plots/avg_exceed_50000_n.png')

# print('-----------------------------------------------------')
# n_samples = 25
# num_batches = len(df2) // n_samples
# for i in range(num_batches):
#     start_index = i * n_samples
#     end_index = min((i + 1) * n_samples, len(df2))
#     batch = df2.iloc[start_index:end_index]
#     # Calculate average, standard deviation, and maximum of the values in the first column

#     avg_time_welzl = batch.iloc[:, 1].mean()
#     avg_time_cgal = batch.iloc[:, 2].mean()
#     avg_time_det = batch.iloc[:, 3].mean()

#     n_points = batch.iloc[0,0]
#     print(f'n: {n_points}, Average runtime for Welzl: {avg_time_welzl}, Cgal: {avg_time_cgal}, Deterministic: {avg_time_det}')

# print('-----------------------------------------------------')

# print('------------------------------------------------\n')