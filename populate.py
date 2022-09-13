import os
import random

def populate(n, r, path, l):
    nums = {}
    total_votes = 0
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
                total_votes += 1
    for key in sorted(nums.keys()):
        print(key, nums[key])
    print('-------------------------------')
    print(max(nums, key=nums.get), nums[max(nums, key=nums.get)])
    print('-------------------------------')
    print('total votes:', total_votes)
populate(25, 10, 'data2/', 30)