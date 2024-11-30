import csv
import matplotlib.pyplot as plt
import numpy as np
import sys

peak_theoretical_bandwidth = 384 * 10**9 # 6 (no.of cores) × 4.0x10^9 (clock rate) × 16(Flops per cycle)
peak_memory_bandwidth = 51.2 * 10**9 # 3.2 (no.of cores) × 8 (bytes) × 2 (channels)
size=[16,32,64,128,256,512,1024,2048,4096]
normal=[0,0,0,0,0,0,0,0,0]
explicit1=[0,0,0,0,0,0,0,0,0]
explicit2=[0,0,0,0,0,0,0,0,0]
explicit3=[0,0,0,0,0,0,0,0,0]
explicit4=[0,0,0,0,0,0,0,0,0]
n=0
with open('bandwidth.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        i=n%9
        normal[i]+=(float(row[0]))
        explicit1[i]+=(float(row[5]))
        explicit2[i]+=(float(row[6]))
        explicit3[i]+=(float(row[7]))
        explicit4[i]+=(float(row[8]))
        n+=1

n/=9
normal=[x/n for x in normal]
explicit1=[x/n for x in explicit1]
explicit2=[x/n for x in explicit2]
explicit3=[x/n for x in explicit3]
explicit4=[x/n for x in explicit4]

arithmetic_intensity = []
for n in size:
    flops = 2 * (n**3)
    bytes_accessed = 3 * (n**2) * 8
    intensity = flops / bytes_accessed
    arithmetic_intensity.append(intensity)

plt.figure(figsize=(10, 6))
plt.yscale('log')
plt.xscale('log', base=2)

plt.plot(arithmetic_intensity, normal, 'o-', label='Normal')
plt.plot(arithmetic_intensity, explicit1, 'o-', label='Method 1')
plt.plot(arithmetic_intensity, explicit2, 'o-', label='Method 2')
plt.plot(arithmetic_intensity, explicit3, 'o-', label='Method 3')
plt.plot(arithmetic_intensity, explicit4, 'o-', label='Method 4')

min_ai = min(arithmetic_intensity)
max_ai = max(arithmetic_intensity)
x_roofline = [min_ai, peak_theoretical_bandwidth/peak_memory_bandwidth, max_ai]
y_roofline = [peak_memory_bandwidth * min_ai, peak_theoretical_bandwidth, peak_theoretical_bandwidth]
plt.plot(x_roofline, y_roofline, 'r--', label='Roofline')

plt.axhline(y=peak_theoretical_bandwidth, color='g', linestyle='--', label='Peak Compute')
plt.axhline(y=peak_memory_bandwidth, color='b', linestyle='--', label='Peak Memory')


plt.xlabel('Arithmetic Intensity (FLOPS/Byte)')
plt.ylabel('Performance (FLOPS/s)')
plt.title('Roofline Model Analysis of Explicit Implementations')
plt.grid(True)
plt.legend()
plt.savefig('roofline_explicit.png', dpi=300, bbox_inches='tight')
