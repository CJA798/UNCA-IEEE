#!/bin/bash


# Set the serial port that the Teensy is connected to
SERIAL_PORT="/dev/ttyACM0"


while true; do
    # Run the Python script
    python edge_communication_test.py

    # Check the exit status of the Python script
    if [ $? -eq 0 ]; then
        echo "communication_test.py exited successfully"
        echo "RESET" > $SERIAL_PORT
        sleep 2
    else
        echo "ERROR: communication_test.py exited with an error. Restarting..."
	    # Send a reset signal to the Teensy
        sleep 2
	    echo "RESET" > $SERIAL_PORT
    fi

    # Wait for a short period before restarting the Python script
    sleep 1
done
