import math
import numpy as np
from numpy.polynomial import polynomial as poly

# from homo import encoder
import encoder

CKKSPair = tuple[np.array, np.array]

def next_pow2(n: int):
    l = math.log2(n)
    return (2 ** math.ceil(l)) if l != int(l) else n
#Should be private as cannot encode
class CKKSCipherText:

    def __init__(self, cipher: CKKSPair, enc: encoder.CKKSEncoder, scale: int):
        self.cipher = cipher
        self.encoder = enc
        self.scale = scale

        #Should make immutable properties
        self.fst = self.cipher[0]
        self.snd = self.cipher[1]

    def __str__(self):
        return f"({self.fst}, {self.snd})"

    #make immutabel dude!
    def add_plain(self, val: np.array): # -> CKKSCipherText:
        encoded_val = self.encoder.encode(val)
        return CKKSCipherText(
                (poly.polyadd(encoded_val, self.fst), self.snd),
                self.encoder, self.scale
                )


    def mul_plain(self, val: np.array): # -> CKKSCipherText:
        encoded_val = self.encoder.encode(val)
        return CKKSCipherText(
                ((1/self.scale) * poly.polymul(encoded_val, self.fst),
                (1/self.scale) * poly.polymul(encoded_val, self.snd)),
                self.encoder, self.scale
                )

    # @staticmethod
def add_cipher(cipherA: CKKSCipherText,
               cipherB: CKKSCipherText) -> CKKSCipherText:
    return CKKSCipherText(
            (cipherA.fst + cipherB.fst, cipherA.snd + cipherB.snd),
            cipherA.encoder, cipherA.scale
            )


def matmul_plain(homo_inputs, kernel, num_outputs):
    output = [None] * num_outputs
    #  reshape vector to be safe (i,e flatten)
    homo_inputs = homo_inputs.reshape(-1)
    #since vector matrix mul theres no need for outer most loop
    for i in range(homo_inputs.shape[0]):
        val = homo_inputs[i]
        for j in range(num_outputs):
            out = output[j]
            if out is None:
                output[j] = val.mul_plain(np.array([kernel[i][j]]))
            else:
                output[j] = add_cipher(output[j], 
                        val.mul_plain(np.array([kernel[i][j]])))
    return output

class CKKSKeys:
    
    def __init__(self, input_size: int, q: int, scale: float = 32768): # 2 ^ 15
        self.input_size = input_size
        self.n = next_pow2(self.input_size * 2)
        self.q = q
        self.scale = scale
        
        rng = np.random.default_rng()
        self.a = rng.integers(-self.q, self.q, size=self.n, endpoint=True) 
        
        secret_scale = self.q - round(math.log2(self.q))
        self.s = rng.integers(-secret_scale, secret_scale,
                size=self.n, endpoint=True)

        #Do normal nexttime
        self.e = rng.integers(-1, 1, size=self.n, endpoint=True)
        self.public_key = (poly.polyadd #Why do i have to split like this?
                (poly.polymul(-self.a, self.s), self.e),
                self.a)

        self.encoder = encoder.CKKSEncoder(self.n * 2, self.scale)


    def encrypt(self, msg: np.array) -> CKKSCipherText:
        encoded_msg = self.encoder.encode(msg)
        return CKKSCipherText((poly.polyadd(encoded_msg, self.public_key[0]), 
                self.public_key[1]), self.encoder, self.scale)

    def decrypt(self, encryted_msg: CKKSCipherText,
            round_scale: int = None, isReal: bool = False) -> np.array:

        decrypted_encoded_msg = poly.polyadd(
                encryted_msg.fst, poly.polymul(encryted_msg.snd, self.s))

        decoded =  self.encoder.decode(decrypted_encoded_msg)
        
        if isReal:
            decoded = np.real(decoded)

        if round_scale is not None:
            decoded = np.round(decoded, round_scale)

        return decoded



