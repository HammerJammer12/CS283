#!/usr/bin/env bats

# File: student_tests.sh
#
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
	run ./dsh <<EOF
ls
EOF

	# Assertions
	[ "$status" -eq 0 ]
}

@test "Check 'pwd' returns expected output" {
	expected_output="$(pwd)dsh2> dsh2> cmd loop returned 0"

	run ./dsh <<EOF
pwd
EOF

	stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

	echo "Captured stdout:"
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "Stripped Output: $stripped_output"
	echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
	[ "$stripped_output" = "$expected_output" ]
}

@test "Check 'cd' changes directory" {
	expected_output="/dsh2> dsh2> dsh2> cmd loop returned 0"

	run ./dsh <<EOF
cd /
pwd
EOF

	stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

	echo "Captured stdout:"
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "Stripped Output: $stripped_output"
	echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
	[ "$stripped_output" = "$expected_output" ]
}

@test "Check non-existent command fails" {
	run ./dsh <<EOF
nonexistentcommand
EOF

		expected_output="CMD_ERR_EXECUTE: No such file or directorydsh2> dsh2> dsh2> cmd loop returned 0"
		stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

	echo "Captured stdout:"
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "Stripped Output: $stripped_output"
	echo "Expected Output: $expected_output"

	[ "$stripped_output" = "$expected_output" ]
}

@test "Check piped command execution" {
	expected_output="hellodsh2> dsh2> cmd loop returned 0"

	run ./dsh <<EOF
echo "hello" | grep "hello"
EOF

	stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

	echo "Captured stdout:"
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "Stripped Output: $stripped_output"
	echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
	[ "$stripped_output" = "$expected_output" ]
}

@test "Check argument parsing" {
	expected_output="arg1 arg2dsh2> dsh2> cmd loop returned 0"

	run ./dsh <<EOF
echo "arg1 arg2"
EOF

	stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

	echo "Captured stdout:"
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "Stripped Output: $stripped_output"
	echo "Expected Output: $expected_output"

	[ "$status" -eq 0 ]
	[ "$stripped_output" = "$expected_output" ]
}

