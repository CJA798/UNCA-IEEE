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

#stuff above if reading what's in the file, will read the current data when pi dies and feed it into the counter values

def updatingData(filename, yellowDucks, pinkDucks, whiteColumns, greenColumns, redColPos: []):
'''    echo "" > filename      #clearing data

    # writing new data to file
    with open(filename, 'w') as f:
        for i in range(0, yellowDucks):     #0 for yellow ducks
            f.write(f'0\n')
        for i in range(0, pinkDucks):       #1 for pink duck
            f.write(f'1\n')
        for i in range(0, whiteColumns):    #2 for white columns
            f.write(f'2\n')
        for i in range(0, greenColumns):    #3 for green columns
            f.write(f'3\n')
        if len(redColPos) > 0 and redColPos[0] == 4
            f.write(f'4, 1\n')          #write any 
        elif len(redColPos) > 1 and redColPos[1] == 4
            f.write(f'4, 2\n')
    '''
    

def shortTowerOne(bigState, towerState, filename, yellowDucks, pinkDucks, whiteColumns, greenColumns, redColPos):
    #store what we now have in drum & states. this gets called whenever something gets pushed out, and we're building the first small tower
    '''echo "" > filename
    
    with open(filename, 'w') as f:
        f.write('2\n')      #writing that we're in big state 2 (collection state)
        f.write('0\n')      #writing that we're in tower state 0 (first small tower)

    updatingData(filename, yellowDucks, pinkDucks, whiteColumns, greenColumns, redColPos)
    '''
        

def shortTowerTwo(bigState, towerState, filename, yellowDucks, pinkDucks, whiteColumns, greenColumns, redColPos):
    #store what we now have in drum & states. this gets called whenever something gets pushed out, and we're building the second small tower
    '''echo "" > filename

    with open(filename, 'w') as f:
        f.write('2\n')      #writing that we're in big state 2 (collection state)
        f.write('1\n')      #writing that we're in tower state 1 (second small tower)

    updatingData(filename, yellowDucks, pinkDucks, whiteColumns, greenColumns, redColPos)
    '''


def bigTower(bigState, towerState, filename, yellowDucks, pinkDucks, whiteColumns, greenColumns, redColPos):
    #store what we now have in drum & states. this gets called whenever something gets pushed out, and we're building the big tower
    '''echo "" > filename

    with open(filename, 'w') as f:
        f.write('2\n')      #writing that we're in big state 2 (collection state)
        f.write('2\n)       #writing that we're in tower state 2 (big tower)

    updatingData(filename, yellowDucks, pinkDucks, whiteColumns, greenColumns, redColPos)
    '''    


def StateStartBackup():
    
    pass

def StateCollecting(filename, YDucks, GCol, WCol, PDuck, RColPos):
    with open(filename, 'w') as f:
        f.write(f'1\n')
        for i in range(0, YDucks):
            f.write(f'0\n')
        for i in range(0, PDuck):
            f.write(f'1\n')
        for i in range(0, WCol):
            f.write(f'2\n')
        for i in range(0, GCol):
            f.write(f'3\n')
        f.write(f'{RColPos}')

def StateTower1():
    pass

def StateTower2():
    pass

def StateTower3():
    pass