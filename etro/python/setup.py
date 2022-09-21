import setuptools
import cemake

setuptools.setup(
    ext_modules = [cemake.CMakeExtension("falcie","../..", ["falpy"])],
    cmdclass = {'build_ext': cemake.cmake_build_ext}
    )
