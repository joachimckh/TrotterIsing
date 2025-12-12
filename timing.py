import numpy as np
import matplotlib.pyplot as plt


size=16
time = np.array([35, 597, 31699, 887720]) # ms
nqubits = np.array([6, 8, 10, 12])

plt.figure(figsize=(6, 4))
plt.plot(nqubits, time, marker='o',color="red")
plt.yscale('log')
plt.xlabel(r'$n_\mathrm{qubits}$',size=size, loc='right')
plt.ylabel('Time (ms)', size=size, loc='top')

plt.annotate("Macbook Pro 32 GB Ram, M1 Max", xy=(0.025, 0.9), xycoords='axes fraction',fontsize=size, color='gray')
plt.tick_params(direction='in', top=True, right=True)
plt.grid(alpha=0.8)


plt.savefig('figures/scaling_time.pdf', bbox_inches='tight', dpi=300)
