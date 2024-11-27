import csv
import matplotlib.pyplot as plt

print(f"Reading data from transpose.csv")

size=[16,32,64,128,256,512,1024,2048,4096]
val1=[]
val2=[]
val3=[]

with open('symmetry.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        val1.append(float(row[0]))
        val2.append(float(row[1]))
        val3.append(float(row[2]))

normal=[]
implicit=[]
explicit=[]
for i in range(0, 9):
    normal.append(val1[i]+val1[i+9]+val1[i+18]+val1[i+27]+val1[i+36]+val1[i+45]+val1[i+54]+val1[i+63]+val1[i+72]+val1[i+81])
    implicit.append(val2[i]+val2[i+9]+val2[i+18]+val2[i+27]+val2[i+36]+val2[i+45]+val2[i+54]+val2[i+63]+val2[i+72]+val2[i+81])
    explicit.append(val3[i]+val3[i+9]+val3[i+18]+val3[i+27]+val3[i+36]+val3[i+45]+val3[i+54]+val3[i+63]+val3[i+72]+val3[i+81])

normal=[x/10 for x in normal]
implicit=[x/10 for x in implicit]
explicit=[x/10 for x in explicit]

for i in range(0, 9):
    normal[i] = size[i] * size[i] * 2 * 4 / normal[i]
    implicit[i] = size[i] * size[i] * 2 * 4 / implicit[i]
    explicit[i] = size[i] * size[i] * 2 * 4 / explicit[i]
    
x = range(len(size))
width = 0.25

plt.bar(x, normal, width, label='Normal', align='center')
plt.bar([i + width for i in x], implicit, width, label='Implicit', align='center')
plt.bar([i + width * 2 for i in x], explicit, width, label='Explicit', align='center')

plt.xlabel('Matrix Size')
plt.ylabel('Bandwidth')
plt.title('Performance Comparison')
plt.legend()
plt.grid(True, alpha=0.3)

plt.xticks([i + width for i in x], size)  # Center x-axis labels

plt.savefig('symmetry.png')

plt.clear()
val1.clear()
val2.clear()
val3.clear()

with open('tranpose.csv', 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        val1.append(float(row[0]))
        val2.append(float(row[1]))
        val3.append(float(row[2]))

normal.clear()
implicit.clear()
explicit.clear()

for i in range(0, 9):
    normal.append(val1[i]+val1[i+9]+val1[i+18]+val1[i+27]+val1[i+36]+val1[i+45]+val1[i+54]+val1[i+63]+val1[i+72]+val1[i+81])
    implicit.append(val2[i]+val2[i+9]+val2[i+18]+val2[i+27]+val2[i+36]+val2[i+45]+val2[i+54]+val2[i+63]+val2[i+72]+val2[i+81])
    explicit.append(val3[i]+val3[i+9]+val3[i+18]+val3[i+27]+val3[i+36]+val3[i+45]+val3[i+54]+val3[i+63]+val3[i+72]+val3[i+81])

normal=[x/10 for x in normal]
implicit=[x/10 for x in implicit]
explicit=[x/10 for x in explicit]

for i in range(0, 9):
    normal[i] = size[i] * size[i] * 2 * 4 / normal[i]
    implicit[i] = size[i] * size[i] * 2 * 4 / implicit[i]
    explicit[i] = size[i] * size[i] * 2 * 4 / explicit[i]
    
x = range(len(size))
width = 0.25

plt.bar(x, normal, width, label='Normal', align='center')
plt.bar([i + width for i in x], implicit, width, label='Implicit', align='center')
plt.bar([i + width * 2 for i in x], explicit, width, label='Explicit', align='center')

plt.xlabel('Matrix Size')
plt.ylabel('Bandwidth')
plt.title('Performance Comparison')
plt.legend()
plt.grid(True, alpha=0.3)

plt.xticks([i + width for i in x], size)  # Center x-axis labels

plt.savefig('tranpose.png')
