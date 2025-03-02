#!/usr/bin/env bats

# File: student_tests.sh
#
# Create your unit tests suit in this file

@test "Pipes: ls | wc -l" {
	run "./dsh" <<EOF
ls | wc -l
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	echo "Captured stdout:"
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "${stripped_output}"

	[ "$status" -eq 0 ]
	[ -n "$output" ]
}


@test "Pipes: cat dshlib.c | grep int | wc -l" {
	run "./dsh" <<EOF
cat dshlib.c | grep int | wc -l
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	echo "Captured stdout:"
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "${stripped_output}"

	[ "$status" -eq 0 ]
	[ -n "$output" ]
}

@test "Pipes: multiple commands ls | grep dsh | sort" {
	run "./dsh" <<EOF
ls | grep dsh | sort
EOF

	stripped_output=$(echo "$output" | tr -d '[:space:]')
	echo "Captured stdout:"
	echo "Output: $output"
	echo "Exit Status: $status"
	echo "${stripped_output}"

	[ "$status" -eq 0 ]
	[ -n "$output" ]
}

