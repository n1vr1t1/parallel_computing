import csv
import matplotlib.pyplot as plt
import sys
import os

if len(sys.argv) != 3:
    print("Usage: python speedup.py <input_csv_file> <output_image_file>")
    sys.exit(1)

if not os.path.exists(sys.argv[1]):
    print(f"Error: Input file '{sys.argv[1]}' does not exist")
    sys.exit(1)

size=[16,32,64,128,256,512,1024,2048,4096]
n=0
implicit1=[0,0,0,0,0,0,0,0,0]
implicit2=[0,0,0,0,0,0,0,0,0]
implicit3=[0,0,0,0,0,0,0,0,0]
implicit4=[0,0,0,0,0,0,0,0,0]
explicit1=[0,0,0,0,0,0,0,0,0]
explicit2=[0,0,0,0,0,0,0,0,0]
explicit3=[0,0,0,0,0,0,0,0,0]
explicit4=[0,0,0,0,0,0,0,0,0]
with open(sys.argv[1], 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        i=n%9
        implicit1[i]+=(float(row[0]))
        implicit2[i]+=(float(row[1]))
        implicit3[i]+=(float(row[2]))
        implicit4[i]+=(float(row[3]))
        explicit1[i]+=(float(row[4]))
        explicit2[i]+=(float(row[5]))
        explicit3[i]+=(float(row[6]))
        explicit4[i]+=(float(row[7]))
        n+=1

n/=9
implicit1=[x/n for x in implicit1]
implicit2=[x/n for x in implicit2]
implicit3=[x/n for x in implicit3]
implicit4=[x/n for x in implicit4]
explicit1=[x/n for x in explicit1]
explicit2=[x/n for x in explicit2]
explicit3=[x/n for x in explicit3]
explicit4=[x/n for x in explicit4]

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 14))

ax1.plot(size, implicit1, 'o-', label='Method 1')
ax1.plot(size, implicit2, 's-', label='Method 2')
ax1.plot(size, implicit3, '^-', label='Method 3')
ax1.plot(size, implicit4, 'D-', label='Method 4')
ax1.set_xlabel('Matrix Size')
ax1.set_ylabel('Speed up')
ax1.set_title('Implicit Method Performance')
ax1.set_xscale('log', base=2)
ax1.legend()
ax1.grid(True)

ax2.plot(size, explicit1, 'o-', label='Method 1')
ax2.plot(size, explicit2, 's-', label='Method 2')
ax2.plot(size, explicit3, '^-', label='Method 3')
ax2.plot(size, explicit4, 'D-', label='Method 4')
ax2.set_xlabel('Matrix Size')
ax2.set_ylabel('Speed Up')
ax2.set_title('Explicit Method Performance')
ax2.set_xscale('log', base=2) 
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.savefig(sys.argv[2], dpi=300, bbox_inches='tight')
