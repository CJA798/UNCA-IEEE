from functools import reduce
camera_working = 1
intake = 3
flipper = 3
sweeper = 3
elevator = 3
top_pusher = 3
bot_pusher = 3
brace = 7

# Assign binary values to each variable
camera_working = format(camera_working, 'b').zfill(1)
intake = format(intake, 'b').zfill(2)
flipper = format(flipper, 'b').zfill(2)
sweeper = format(sweeper, 'b').zfill(2)
elevator = format(elevator, 'b').zfill(2)
top_pusher = format(top_pusher, 'b').zfill(2)
bot_pusher = format(bot_pusher, 'b').zfill(2)
brace = format(brace, 'b').zfill(3)

# Concatenate the binary values
result = camera_working + intake + flipper + sweeper + elevator + top_pusher + bot_pusher + brace

# Print the result
print(result)
print(len(result))



variable_status = [1, 3, 3, 3, 3, 3, 3, 7]
bits_per_variable = [1, 2, 2, 2, 2, 2, 2, 3]

# Convert variables to binary strings and pad with zeros
binary_vars = [format(var, 'b').zfill(bits) for var, bits in zip(variable_status, bits_per_variable)]

# Concatenate binary strings and convert back to integer
binary_sum = reduce(lambda concat, binary: concat + binary, binary_vars)

# Print the result
print(binary_sum)
binary_sum = binary_sum.encode()
print(binary_sum)
