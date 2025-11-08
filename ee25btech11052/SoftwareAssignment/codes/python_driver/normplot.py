import matplotlib.pyplot as plt

k_values = []
norm_values = []

with open("/home/shriyasnh/Desktop/SoftwareAssignment/code/globe.txt", "r") as f:
    for line in f:
        parts = line.strip().split()
        if len(parts) == 2:
            k, norm = parts
            k_values.append(int(k))
            norm_values.append(float(norm))

plt.figure(figsize=(8, 6))
plt.plot(k_values, norm_values, marker='o', linestyle='-', linewidth=2)
plt.title("Frobenius Norm vs K Value")
plt.xlabel("K Value")
plt.ylabel("Frobenius Norm")
plt.grid(True)
plt.show()
plt.savefig("/home/shriyasnh/Desktop/SoftwareAssignment/code/Frobenius Norm vs K Value for einstein")
