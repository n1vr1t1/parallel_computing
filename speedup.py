import csv
import matplotlib.pyplot as plt
import sys
import os

# Check if correct number of arguments is provided
if len(sys.argv) != 3:
    print("Usage: python speedup.py <input_csv_file> <output_image_file>")
    sys.exit(1)

# Check if input file exists
if not os.path.exists(sys.argv[1]):
    print(f"Error: Input file '{sys.argv[1]}' does not exist")
    sys.exit(1)

size=[16,32,64,128,256,512,1024,2048,4096]
implicit1=[]
implicit2=[]
implicit3=[]
implicit4=[]
explicit1=[]
explicit2=[]
explicit3=[]
explicit4=[]

with open(sys.argv[1], 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        implicit1.append(float(row[0]))
        implicit2.append(float(row[1]))
        implicit3.append(float(row[2]))
        implicit4.append(float(row[3]))
        explicit1.append(float(row[4]))
        explicit2.append(float(row[5]))
        explicit3.append(float(row[6]))
        explicit4.append(float(row[7]))

# Create figure with two subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 14))

# Plot implicit data as lines
ax1.plot(size, implicit1, 'o-', label='Method 1')
ax1.plot(size, implicit2, 's-', label='Method 2')
ax1.plot(size, implicit3, '^-', label='Method 3')
ax1.plot(size, implicit4, 'D-', label='Method 4')
ax1.set_xlabel('Matrix Size')
ax1.set_ylabel('Speed up')
ax1.set_title('Implicit Method Performance')
ax1.set_xscale('log', base=2)  # Set logarithmic scale with base 2
ax1.legend()
ax1.grid(True, alpha=0.3)

# Plot explicit data as lines
ax2.plot(size, explicit1, 'o-', label='Method 1')
ax2.plot(size, explicit2, 's-', label='Method 2')
ax2.plot(size, explicit3, '^-', label='Method 3')
ax2.plot(size, explicit4, 'D-', label='Method 4')
ax2.set_xlabel('Matrix Size')
ax2.set_ylabel('Speed Up')
ax2.set_title('Explicit Method Performance')
ax2.set_xscale('log', base=2)  # Set logarithmic scale with base 2
ax2.legend()
ax2.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig(sys.argv[2], dpi=300, bbox_inches='tight')  # Save with high resolution
plt.show()  # This will display the plot window
