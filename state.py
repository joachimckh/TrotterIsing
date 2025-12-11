import numpy as np
import matplotlib.pyplot as plt

data =  np.loadtxt("magnetization.txt")
#params = np.loadtxt("input_params.txt")

mSize = 20
nqubits = 6
M = 200
t = 40
delta = t/M
g = 1.0

x = [i*delta for i in range(M+1)]

f = plt.figure(figsize=(8,6))
ax = plt.gca()

ax.plot(x,data[:,0],'--', label="Exact", color="black")
ax.plot(x,data[:,1], label="Trotter", color="darkred")

ax.legend(loc = "upper right", prop={'size': mSize}, frameon=False)

plt.tick_params(direction='in', top=True, right=True)
plt.grid(alpha=0.2)
ax.set_xlabel("t", fontsize=mSize, loc ='right')
ax.set_ylabel(r"Magnetization", fontsize=mSize, labelpad=2, loc='top')

# newline in annotate with f'' string ? 
ax.annotate(
  f"$n_\\mathrm{{qubits}}={nqubits}$\n$g={g}$\n$\\delta={delta}$",
  xy=(0.02, 0.1),
  xycoords='axes fraction',
  fontsize=mSize
)

plt.tight_layout()
plt.show()

f.savefig("ising_trotter.png")
