import matplotlib.pyplot as plt
import os
import csv

# Get path to GPS.csv in the script's directory
script_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(script_dir, "GPS.csv")

# Lists to store GPS coordinates
latitudes = []
longitudes = []

# Read GPS data
with open(file_path, "r") as file:
    reader = csv.reader(file)
    for row in reader:
        if len(row) < 8:
            continue  # skip incomplete lines
        try:
            lat_raw = int(row[6])
            lon_raw = int(row[7])

            # Convert from scaled int to float degrees
            lat = lat_raw / 1e6
            lon = lon_raw / 1e6

            latitudes.append(lat)
            longitudes.append(lon)
        except ValueError:
            continue  # skip malformed lines

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(longitudes, latitudes, marker='o', linestyle='-')
plt.xlabel("Longitude")
plt.ylabel("Latitude")
plt.title("GPS Path (Latitude vs. Longitude)")
plt.grid(True)
plt.axis("equal")  # Preserve aspect ratio
plt.tight_layout()
plt.show()
