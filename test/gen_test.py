import random

num_test_cases = 1

# N [2; 100 000]
num_stalls = 20
# C [2; N]
num_cows = 5

max_min_distance = 5

stall_positions = []
cur_stall_position = -1
for idx in range(num_stalls):
    cur_stall_position += random.randint(1, max_min_distance)
    stall_positions.append(cur_stall_position)
    

print("stall_positions = [%s]" % stall_positions)

with open('testcase.txt', 'w') as fp:
    fp.write("%d\n" % num_test_cases)
    fp.write("%d %d\n" % (num_stalls, num_cows))
    for item in stall_positions:
        fp.write("%d\n" % item)