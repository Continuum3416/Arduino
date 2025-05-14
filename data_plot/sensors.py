import matplotlib.pyplot as plt
import os
import csv

# Get path to LOG.csv in the script's directory
script_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(script_dir, "LOG.csv")

# Lists to store parsed data
times, humidity, pressure, altitude, temperature = [], [], [], [], []

# Read the new CSV format
with open(file_path, "r") as file:
    reader = csv.reader(file)
    for row in reader:
        if len(row) < 6:
            continue  # skip incomplete rows

        date, time_str, h, p, a, t = row[:6]
        times.append(time_str)
        humidity.append(float(h))
        pressure.append(float(p))
        altitude.append(float(a))
        temperature.append(float(t))

# Plotting
plt.figure(figsize=(12, 10))

plt.subplot(4, 1, 1)
plt.plot(times, humidity, marker='o')
plt.ylabel('Humidity (H)')
plt.xticks(rotation=45)

plt.subplot(4, 1, 2)
plt.plot(times, pressure, marker='o', color='orange')
plt.ylabel('Pressure (P)')
plt.xticks(rotation=45)

plt.subplot(4, 1, 3)
plt.plot(times, altitude, marker='o', color='green')
plt.ylabel('Altitude (A)')
plt.xticks(rotation=45)

plt.subplot(4, 1, 4)
plt.plot(times, temperature, marker='o', color='red')
plt.ylabel('Temperature (T)')
plt.xlabel('Time (HH:MM:SS)')
plt.xticks(rotation=45)

plt.tight_layout()
plt.show()
