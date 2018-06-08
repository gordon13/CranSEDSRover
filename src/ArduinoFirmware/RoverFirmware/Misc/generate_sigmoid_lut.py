import matplotlib.pyplot as plt
import math

samples = 100
lut = []


def normalise(value):
    return value * (samples - 0) / samples


for i in range(samples):
    speed = 0.5 + 0.5 * math.tanh((normalise(i) - (samples / 2)) * 0.05)
    # B+δg((x−B)/δ)
    lut.append((i, speed))
    print(speed)


t = range(samples)
speed = [p[1] for p in lut]
plt.plot(t, speed)
plt.show()
