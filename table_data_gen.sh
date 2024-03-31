#!/bin/bash

# Define the executable name
# executable="/home/hitesh03/Desktop/Academics/CS648/CS648-Mini-Project/welzl_compare_time"     #Uncomment this for data related to Table 1,2
# executable="/home/hitesh03/Desktop/Academics/CS648/CS648-Mini-Project/sec2"        #Uncomment this for data related to Table 3
executable="/home/hitesh03/Desktop/Academics/CS648/CS648-Mini-Project/compare_all_four"        #Uncomment this for data related to Table 3

# Define the list of the values of n (number of points)
numerical_args=(5 10 30 50 80 100 200 400)        #Uncomment this for data related to Table 1,2
# numerical_args=(5 10 20 50 80 100 200 500 1000 2000 5000 10000 50000)          #Uncomment this for data related to Table 3
# numerical_args=()
n=2000

# for ((i = 1; i <= n; i++)); do
#     numerical_args+=($i)
# done

# Fixed numerical argument
num_samples=25       #Number of samples to be generated for each value of n

# file_to_be_deleted="data_report_tables/data_table_1.csv"        #Uncomment this for data related to Table 1,2
file_to_be_deleted="data_report_tables/data_table_3.csv"        #Uncomment this for data related to Table 3
# file_to_be_deleted="data_report_tables/data_table_sec.csv"        #Uncomment this for data related to Table 3

rm "-f" "$file_to_be_deleted"

# Run the executable with each numerical argument from the list
for arg in "${numerical_args[@]}"; do
    "$executable" "$arg" "$num_samples"
done
