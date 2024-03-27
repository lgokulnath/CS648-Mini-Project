#!/bin/bash

# Define the executable name
# executable="/home/hitesh03/Desktop/Academics/CS648/CS648-Mini-Project/welzl_compare_time"     #Uncomment this for data related to Table 1,2
# executable="/home/hitesh03/Desktop/Academics/CS648/CS648-Mini-Project/compare_all_three"        #Uncomment this for data related to Table 3
executable="/home/hitesh03/Desktop/Academics/CS648/CS648-Mini-Project/sec2"        #Uncomment this for data related to Table 3

# Define the list of the values of n (number of points)
# numerical_args=(5 10 20 30 40 50 60 70 80 90 100 200 300 400 500 1000)        #Uncomment this for data related to Table 1,2
numerical_args=(5 10 20 50 80 100 200 500 1000)          #Uncomment this for data related to Table 3

# Fixed numerical argument
num_samples=1000         #Number of samples to be generated for each value of n

# file_to_be_deleted="data_report_tables/data_table_1.csv"        #Uncomment this for data related to Table 1,2
# file_to_be_deleted="data_report_tables/data_table_3.csv"        #Uncomment this for data related to Table 3
file_to_be_deleted="data_report_tables/data_table_sec.csv"        #Uncomment this for data related to Table 3

rm "$file_to_be_deleted"

# Run the executable with each numerical argument from the list
for arg in "${numerical_args[@]}"; do
    "$executable" "$arg" "$num_samples"
done
