import setuptools
from cemake.cemake_extension import CeMakeExtension

setuptools.setup(
    ext_modules = [CeMakeExtension("falcie","C:\\Users\\james\\faldroid\\falcie", ["falpy"])],
    )
