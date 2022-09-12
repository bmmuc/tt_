# creates a function that recieve four arguments
# the first argument is the number of files to be created.
# the second argument is the range of the numbers to be write in the files.
# the third argument is the path where the files will be created.
# the fourth argument is the number of lines to be write in the files.
# each line has a number between 0 and the range.
# a file can has a number of lines between 1 and the number of lines.
import os
import random

def populate(n, r, path, l):
    nums = {}
    for i in range(1,n + 1):
        with open(os.path.join(path, str(i) + ".in"), "w") as f:
            lines = random.randint(1, l)
            f.write(str(lines) + "\n")
            for j in range(lines):
                num = random.randint(0, r)
                f.write(str(num) + "\n")
                if num in nums:
                    nums[num] += 1
                else:
                    nums[num] = 1

    for key in sorted(nums.keys()):
        print(key, nums[key])
    # find the key with the highest value
    print('-------------------------------')
    print(max(nums, key=nums.get), nums[max(nums, key=nums.get)])
    print('-------------------------------')

populate(25, 10, 'data2/', 30)