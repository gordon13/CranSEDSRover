import matplotlib.pyplot as plt
import lut

# variables
total_iterations = 500
offset = 113
currentspeed = 80
targetspeed = 200
maxspeed = 255
accelerationtime = 5
dt = 0.1
print("length of simulation [s] ", dt * total_iterations)
print("dt [s]", dt)
print("acceleration iterations [iterations]", accelerationtime / dt)

# lut
sigmoid = lut.sigmoid
number_lut_points = len(sigmoid)
sigmoid_sample_interval = accelerationtime / number_lut_points  # acceleration time interval
max_number_of_samples_possible_in_time = int(accelerationtime / dt)
sample_every = int(number_lut_points / max_number_of_samples_possible_in_time)
print("sigmoid_sample_interval ", sigmoid_sample_interval)
print("max samples possible ", max_number_of_samples_possible_in_time)
print("sample_every ", sample_every)

# main code
points = []
speed_difference = targetspeed - currentspeed
newspeed = currentspeed
sigmoid_i = 0
for i in range(total_iterations):
    newspeed = currentspeed + speed_difference * sigmoid[sigmoid_i]
    # if (i % sample_every == 0):
    if (sigmoid_i < len(sigmoid)):
        if (i % max_number_of_samples_possible_in_time == 0):
            sigmoid_i += sample_every
        sigmoid_i = max(0, min(sigmoid_i, len(sigmoid) - 1))
    points.append((i, newspeed))


t = range(total_iterations)
speed = [p[1] for p in points]
plt.plot(t, speed)
plt.show()
