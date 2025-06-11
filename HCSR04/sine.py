import math
from vpython import gcurve, graph, color, rate

# Create a graph window
graph1 = graph(title='Sine Wave: 3 Cycles, Amplitude 2m, Wavelength 2m',
               xtitle='Position (m)', ytitle='Displacement (m)',
               xmin=-3, xmax=3, ymin=-2.5, ymax=2.5,
               foreground=color.black, background=color.white)

# Create the curve object
sine_wave = gcurve(color=color.red, graph=graph1)

# Constants
amplitude = 2.0       # meters
wavelength = 2.0      # meters
k = 2 * math.pi / wavelength

# Plotting 3 full cycles
x_start = -3.0
x_end = 3.0
step = 0.01

# Use a for loop with range and calculated number of steps
num_steps = int((x_end - x_start) / step) + 1
while True:
  rate(20)
  
  for i in range(num_steps):
    x = x_start + i * step
    y = amplitude * math.sin(k * x)
    sine_wave.plot(pos=(x, y))