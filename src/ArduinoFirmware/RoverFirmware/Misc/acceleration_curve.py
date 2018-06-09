import matplotlib.pyplot as plt
import lut

# variables
total_iterations = 500
currentspeed = 80  # digital value
targetspeed = 200  # digital value
maxspeed = 255  # digital value
dt = 0.1  # s
totaltime = total_iterations * dt  # s
accelerationtime = 5  # s
acceleration_iterations = int(accelerationtime / dt)

print("total time", totaltime)
print("acceleration time", accelerationtime)
print("")
print("total iterations", total_iterations)
print("acceleration_iterations", acceleration_iterations)
print("")


# lut
sigmoid = lut.sigmoid
number_lut_points = len(sigmoid)
sample_every = int(number_lut_points / acceleration_iterations)

print("sigmoid points", number_lut_points)
print("sample every", sample_every)



# main code
points = []
speed_difference = targetspeed - currentspeed
newspeed = currentspeed
sigmoid_i = 0
for i in range(total_iterations):
    newspeed = currentspeed + speed_difference * sigmoid[sigmoid_i]
    if (sigmoid_i + sample_every < len(sigmoid)):
        sigmoid_i += sample_every
    points.append((i, newspeed))


t = range(total_iterations)
speed = [p[1] for p in points]
plt.plot(t, speed)
plt.ylim(0, maxspeed)
plt.ylabel("Speed value")
plt.xlabel("Iterations")
plt.show()
