import matplotlib.pyplot as plt
import math

import lut

# variables
total_iterations = 500
offset = 0
currentspeed = 80
targetspeed = 200
maxspeed = 255
accelerationtime = 2
dt = 0.1
print("dt ", dt)
# lut
sigmoid = lut.sigmoid
number_samples = len(sigmoid)
adt = accelerationtime / number_samples  # acceleration time interval
max_number_of_samples_possible_in_time = int(accelerationtime / dt)
sample_every = int(number_samples / max_number_of_samples_possible_in_time)
print("adt ", adt)
print("max samples possible ", max_number_of_samples_possible_in_time)
print("sample_every ", sample_every)


def normalise():
    pass


points = []
speed_difference = targetspeed - currentspeed
t = 0
for i in range(total_iterations):
    if (i + offset < len(sigmoid)):
        if (i % sample_every == 0):
            newspeed = currentspeed + speed_difference * sigmoid[i]
    points.append((i, newspeed))
    # print(i)
    t += dt


t = range(total_iterations)
speed = [p[1] for p in points]
plt.plot(t, speed)
plt.show()


0.1, 0.02
0.2, 0.02
0.3, 0.02
