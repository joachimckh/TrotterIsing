import numpy as np
import matplotlib.pyplot as plt

data =  np.loadtxt("magnetization.txt")
#params = np.loadtxt("input_params.txt")
#load params
with open("input_params.txt", "r") as f:
  lines = f.readlines()
  params = {}
  for line in lines:
    key, value = line.split(" ")
    params[key.strip()] = float(value.strip())

mSize = 20
nqubits = int(params["nqubits"])
M = int(params["M"])
t =  params["t"]
delta = t/M
g = params["g"]

x = [i*delta for i in range(M+1)]

#### plot 1
f = plt.figure(figsize=(8,6))
ax = plt.gca()

ax.plot(x,data[:,0],'--', label="Exact", color="black")
ax.plot(x,data[:,1], 'o-',markersize=3, label="Trotter", color="darkred")

ax.legend(loc = "upper right", prop={'size': mSize}, frameon=False)

ax.set_ylabel(r"Magnetization", fontsize=mSize, labelpad=2, loc='top')

# newline in annotate with f'' string ? 
ax.annotate(
  f"$n_\\mathrm{{qubits}}={nqubits}$\n$g={g}$\n$\\delta={delta}$",
  xy=(0.02, 0.1),
  xycoords='axes fraction',
  fontsize=mSize
)

#### plot 2 Error
error = np.abs(data[:,0] - data[:,1])
f2 = plt.figure(figsize=(8,6))
ax2 = plt.gca()

ax2.plot(x, error, 'o-',markersize=3, color="black", label=r"$M_\mathrm{Exact} - M_\mathrm{Trotter 1st order}$")
ax2.legend(loc = "upper left", prop={'size': mSize}, frameon=False)
ax2.set_ylabel(r"Error", fontsize=mSize, labelpad=2, loc='top')

#### plot 3 Fidelity
f3 = plt.figure(figsize=(8,6))
ax3 = plt.gca()
ax3.plot(x, data[:,2], 'o-',markersize=3, color="black", label=r"Trotter 1st order")
ax3.set_ylabel(r"Fidelity", fontsize=mSize, labelpad=2, loc='top')
ax3.legend(loc = "upper right", prop={'size': mSize}, frameon=False)
ax3.set_ylim(0,1)

axs = [ax, ax2, ax3]
for a in axs:
  a.tick_params(direction='in', top=True, right=True)
  a.grid(alpha=0.2)
  a.set_xlabel("t", fontsize=mSize, loc ='right', labelpad=2)
  a.set_xlim(0, t)
f.savefig("figures/ising_trotter.png", bbox_inches='tight')
f2.savefig("figures/ising_trotter_error.png", bbox_inches='tight')
f3.savefig("figures/fidelity.png", bbox_inches='tight')
