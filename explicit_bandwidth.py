import csv
import matplotlib.pyplot as plt
import numpy as np
import sys

peak_theoretical_bandwidth = 384 * 10**9 #6(no.of cores)×4.0x10^9(clock rate)×16(Flops per cycle)
peak_memory_bandwidth = 51.2 * 10**9 #3.2(no.of cores)×8(bytes)×2 (channels)
size=[16,32,64,128,256,512,1024,2048,4096]
normal=[]
explicit1=[]
explicit2=[]
explicit3=[]
explicit4=[]

with open('bandwidth.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        normal.append(float(row[0]))
        explicit1.append(float(row[5]))
        explicit2.append(float(row[6]))
        explicit3.append(float(row[7]))
        explicit4.append(float(row[8]))
# Calculate arithmetic intensity (Operations per byte)
# For matrix multiplication: 2N^3 operations and 3N^2 memory accesses
arithmetic_intensity = []
for n in size:
    flops = 2 * (n**3)  # Number of floating point operations
    bytes_accessed = 3 * (n**2) * 8  # Memory accesses in bytes (8 bytes per double)
    intensity = flops / bytes_accessed
    arithmetic_intensity.append(intensity)

# Create figure with logarithmic scales
plt.figure(figsize=(10, 6))
plt.yscale('log')
plt.xscale('log', base=2)

# Plot bandwidth measurements
plt.plot(arithmetic_intensity, normal, 'o-', label='Normal')
plt.plot(arithmetic_intensity, explicit1, 'o-', label='Method 1')
plt.plot(arithmetic_intensity, explicit2, 'o-', label='Method 2')
plt.plot(arithmetic_intensity, explicit3, 'o-', label='Method 3')
plt.plot(arithmetic_intensity, explicit4, 'o-', label='Method 4')

# Plot roofline model
min_ai = min(arithmetic_intensity)
max_ai = max(arithmetic_intensity)
x_roofline = [min_ai, peak_theoretical_bandwidth/peak_memory_bandwidth, max_ai]
y_roofline = [peak_memory_bandwidth * min_ai, peak_theoretical_bandwidth, peak_theoretical_bandwidth]
plt.plot(x_roofline, y_roofline, 'r--', label='Roofline')

# Add peak bandwidth lines
plt.axhline(y=peak_theoretical_bandwidth, color='g', linestyle='--', label='Peak Compute')
plt.axhline(y=peak_memory_bandwidth, color='b', linestyle='--', label='Peak Memory')


plt.xlabel('Arithmetic Intensity (FLOPS/Byte)')
plt.ylabel('Performance (FLOPS/s)')
plt.title('Roofline Model Analysis of Explicit Implementations')
plt.grid(True)
plt.legend()

plt.show()
