print("Start program")
filename = 'drum_data.txt'

# Read data from file and split into lines
with open(filename, 'r') as f:
    lines = f.read().splitlines()

# Initialize variables for different item classes
a = []
b = []
c = []

# Loop through lines and store data in appropriate variables
for line in lines:
    data = line.split(',')
    item_class = int(data[0])
    slot = int(data[1])
    position = int(data[2])
    if item_class == 0:
        a.append((slot, position))
    elif item_class == 1:
        b.append((slot, position))
    else:
        c.append((slot, position))
print("Data read")
print(a)
print(b)
print(c)

# Perform some processing on the data (e.g. sort lists, etc.)
# ...
a.append((2,2))
a = sorted(a)
b = sorted(b)
c.append((0,0))
c = sorted(c)

print("Data modified")


# Write updated data back to file
with open(filename, 'w') as f:
    for item in a:
        f.write(f'0,{item[0]},{item[1]}\n')
    for item in b:
        f.write(f'1,{item[0]},{item[1]}\n')
    for item in c:
        f.write(f'2,{item[0]},{item[1]}\n')

print("Data written")
print(a)
print(b)
print(c)