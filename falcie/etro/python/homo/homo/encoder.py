
import math
import numpy as np
from numpy.polynomial import Polynomial
from numpy.polynomial import polynomial as poly

def isPow2(x: int):
    return math.log2(x) == int(math.log2(x))

def vandermonde(xi: np.complex128, M: int) -> np.array:
    N = M // 2
    matrix = np.zeros((N,N), dtype=np.complex128)
    for i in range(N):
        root = xi ** (2 * i + 1)
        row = []
        for j in range(N):
            row.append(root ** j)
        matrix[i] = row
    return matrix

def round_coordinates(coordinates):
    return coordinates - np.floor(coordinates)

def cordinate_wise_random_rounding(coordinates):
    r = round_coordinates(coordinates)
    f = np.array([np.random.choice([c, c-1], 1, p=[1-c, c]) 
                for c in r]).reshape(-1)

    rounded_coordinates = coordinates - f
    return np.array([int(coeff) for coeff in rounded_coordinates])


class CKKSEncoder:

    def __init__(self, M: int, scale: float = 32768): # 2 ^ 15
        self.xi = np.exp(2 * np.pi * 1j / M)
        self.vand = vandermonde(self.xi, M)
        self.M = M
        self.create_sigma_R_basis()
        self.scale = scale

    def encode(self, z: np.array) -> np.array:
        pi_z = self.pi_inverse(z)
        scaled_pi_z = self.scale * pi_z
        rounded_scale_pi_z = self.sigma_R_discretization(scaled_pi_z)
        p = self.sigma_inverse(rounded_scale_pi_z)

        coef = np.round(np.real(p)).astype(int)
        return coef

    def decode(self, p: np.array) -> np.array:
        rescaled_p = p / self.scale
        z = self.sigma(rescaled_p)
        pi_z = self.pi(z)
        return pi_z


    def sigma_inverse(self, b: np.array) -> np.array:
        coeffs = np.linalg.solve(self.vand, b)
        return coeffs

    def sigma(self, p: np.array) -> np.array:
        outputs = []
        N = self.M // 2

        for i in range(N):
            root = self.xi ** (2 * i + 1)
            outputs.append(poly.polyval(root, p))
        return np.array(outputs)

    def pi(self, z: np.array) -> np.array:
        N = self.M // 4 # N = m/2 and we want that / 2 aka /4
        return z[:N]

    def pi_inverse(self, z: np.array) -> np.array:
        z_conjugate = [np.conjugate(x) for x in z[::-1]]
        return np.concatenate([z, z_conjugate])

    def create_sigma_R_basis(self):
        self.sigma_R_basis = np.array(vandermonde(self.xi, self.M)).T

    def compute_basis_coordinates(self, z):
        return np.array([np.real(np.vdot(z, b) / np.vdot(b,b)) 
                for b in self.sigma_R_basis])

    def sigma_R_discretization(self, z):
        coordinates = self.compute_basis_coordinates(z)

        rounded_coordinates = cordinate_wise_random_rounding(coordinates)
        return np.matmul(self.sigma_R_basis.T, rounded_coordinates)


