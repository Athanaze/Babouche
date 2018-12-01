import matplotlib.pyplot as plt


FPS_LOG_PATH = "../../out/logFiles/fpsLog.data"

with open(FPS_LOG_PATH) as f:
    lines = f.readlines()[:-2]
fps_values = []
for l in lines:
    fps_values.append(float(l[:-2]))
plt.plot(range(len(fps_values)), fps_values)
plt.xlabel("# Model update")
plt.ylabel("FPS")
plt.show()
