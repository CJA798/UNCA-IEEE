print("Start program")
filename = 'drum_data.txt'

# Read data from file and split into lines
with open(filename, 'r') as f:
    lines = f.read().splitlines()

# Initialize variables for different item classes
a = []
b = []
c = []
d = []
e = []

# Loop through lines and store data in appropriate variables
for line in lines:
    data = line.split(',')
    item_class = int(data[0])
    if len(data) > 1:
        position = int(data[1])
        e.append((4, position))
    if item_class == 0:
        a.append(0)
    elif item_class == 1:
        b.append(1)
    elif item_class == 2:
        c.append(2)
    elif item_class == 3:
        d.append(3)
        

print("Data read")
print(a)
print(b)
print(c)
print(d)
print(e)

# Perform some processing on the data (e.g. sort lists, etc.)
# ...
a.append(0)
c.append(2)
e.append((4,0))

print("Data modified")


# Write updated data back to file
with open(filename, 'w') as f:
    for item in a:
        f.write(f'0\n')
    for item in b:
        f.write(f'1\n')
    for item in c:
        f.write(f'2\n')
    for item in d:
        f.write(f'3\n')
    for item in e:
        f.write(f'4,{item[1]}\n')

print("Data written")
print(a)
print(b)
print(c)
print(d)
print(e)