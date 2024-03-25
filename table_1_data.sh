#!/bin/bash

# Define the executable name
executable="/home/hitesh03/Desktop/Academics/CS648/CS648-Mini-Project/welzl_compare_time"

# Define the list of numerical arguments
numerical_args=(5 10 20 30 40 50 60 70 80 90 100 200 300 400 500 1000)

# Fixed numerical argument
fixed_argument=100
file_to_be_deleted="data_report_tables/data_table_1.csv"

rm "$file_to_be_deleted"

# Run the executable with each numerical argument from the list
for arg in "${numerical_args[@]}"; do
    "$executable" "$arg" "$fixed_argument"
done
