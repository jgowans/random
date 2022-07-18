#!/usr/bin/env bash

function set_array_values_first() {
    my_array["first_0"]="hello"
    my_array["first_1"]="world"
}

function set_array_values_second() {
    my_array["second_0"]="james"
    my_array["second_1"]="was here"
}

declare -A my_array

echo "Before modifying array is:"
echo "${!my_array[@]} ${my_array[@]}"
set_array_values_first
echo "After first array is:"
echo "${!my_array[@]} ${my_array[@]}"
set_array_values_second
echo "After second array is"
echo "${!my_array[@]} ${my_array[@]}"

# can I clear an array?
my_array=()
echo "attempting to clear by assignment, array is:"
echo "${!my_array[@]} ${my_array[@]}"

echo "now inoking second again"
set_array_values_second
echo "${!my_array[@]} ${my_array[@]}"
