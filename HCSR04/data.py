import serial  # Import Serial Library, pip install pyserial
from vpython import *  # Import all the vPython library, pip install vpython

# Create an object for the Serial port. Adjust 'com11' to the correct port
arduinoSerialData = serial.Serial('com4', 9600)

# Create the measuring rod and other visual elements
measuringRod = cylinder(radius=0.1, length=6, color=color.yellow, pos=vector(-3, -2, 0))
lengthLabel = label(pos=vector(0, 5, 0), text='Target Distance is: ', box=False, height=30)
target = box(pos=vector(0, -0.5, 0), length=0.2, width=3, height=3, color=color.green)

# Main loop
while True:
    rate(20)  # Run the loop 20 times a second

    if arduinoSerialData.in_waiting > 0:  # Check for available data
        myData = arduinoSerialData.readline().decode().strip()  # Read and decode serial data
        print(myData)  # Debug: Print the received data
        try:
            distance = float(myData)  # Convert string to float
            measuringRod.length = distance  # Update measuring rod length
            target.pos = vector(-3 + distance, -0.5, 0)  # Update target position
            myLabel = 'Target Distance is: ' + myData  # Update label text
            lengthLabel.text = myLabel
        except ValueError:
            print("Invalid data received:", myData)  # Handle non-numeric input