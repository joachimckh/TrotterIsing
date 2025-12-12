import numpy as np
import psutil
import matplotlib.pyplot as plt



ns = np.arange(1, 30)

#read mem.txt
mems = []
with open('qubitscaling/mem.txt', 'r') as f:
    for line in f:
        mems.append(float(line.strip()))
mems = np.array(mems)


size = 16
f = plt.figure(figsize=(6,4))
print(mems)
plt.plot(ns, mems, marker='o', color='red')
#plt.vlines(29, 0, 32, color='red', linestyle='--')
#plt.text(19, 8, '  Threshold for 8MB RAM', va='bottom', ha='left', color='red')
plt.xlabel('number of qubits', loc='right',size = size)
plt.ylabel('memory used (MB)', loc='top',size = size)
# log y
plt.yscale('log')
plt.annotate("Macbook Pro 32 GB Ram, M1 Max", xy=(0.025, 0.9), xycoords='axes fraction',fontsize=size, color='gray')
plt.xlim(9.5,32)
plt.ylim(0.01, 12000)
#plt.annotate("Generated in C++", xy=(0.025, 0.85), xycoords='axes fraction',fontsize=8, color='gray')
plt.tick_params(direction='in', top=True, right=True)
plt.grid(alpha=0.8)
plt.tight_layout()
plt.show()

f.savefig('figures/scaling_memory.pdf', dpi=300)
