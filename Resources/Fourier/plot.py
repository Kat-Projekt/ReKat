import numpy as np
import matplotlib.pyplot as plt
from data import *

# plot
fig, ax = plt.subplots(2,2)

#ax.set(xlim=(0, 300), xticks=np.arange(1, 6), ylim=(0, 300), yticks=np.arange(1, 11))
n = len(data1[1])
x = 0.5 + np.arange(n)
ax[0,0].plot (x,data1[1])
ax[0,0].set_title(data1[0])

n = len(data2[1])
x = 0.5 + np.arange(n)
ax[0,1].plot (x,data2[1])
ax[0,1].set_title(data2[0])

n = len(data3[1])
x = 0.5 + np.arange(n)
ax[1,0].plot (x,data3[1])
ax[1,0].set_title(data3[0])

n = len(data4[1])
x = 0.5 + np.arange(n)
ax[1,1].plot (x,data4[1])
ax[1,1].set_title(data4[0])

plt.show()