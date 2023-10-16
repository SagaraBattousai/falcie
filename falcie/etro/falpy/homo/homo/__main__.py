
import numpy as np
from homo import cipher

# keys = cipher.CKKSKeys(4, 13, 32768) #issue that array size has to be pow 2 :(
# msg = np.array([3.14159653, -2.71828182, 6, 0])


keys = cipher.CKKSKeys(1, 37, 32768) #issue that array size has to be pow 2 :(
msg = np.array([3.14159653])

ct = keys.encrypt(msg)
ct.mul_plain(np.array([6]))

dt = keys.decrypt(ct, 8, True)

print(msg)
print(dt)


keys = cipher.CKKSKeys(1, 13, (2 ** 15)) #issue that array size has to be pow 2 :(
I1 = np.array([1])
I2 = np.array([0])

C1 = keys.encrypt(I1)
C2 = keys.encrypt(I2)

ho = cipher.matmul_plain(np.array([C1,C2]), np.array([[4,5,6],[7,8,9]]), 3)

print("###############################")

for x in ho:
    print(keys.decrypt(x, None, True))# * keys.scale)

