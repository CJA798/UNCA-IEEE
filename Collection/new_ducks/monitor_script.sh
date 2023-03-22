#!/bin/bash

while true; do
    # Run the Python script
    python communication_test.py

    # Check the exit status of the Python script
    if [ $? -eq 0 ]; then
        echo "communication_test.py exited successfully"
    else
        echo "ERROR: communication_test.py exited with an error. Restarting..."
    fi

    # Wait for a short period before restarting the Python script
    sleep 1
done
