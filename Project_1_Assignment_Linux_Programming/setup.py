from setuptools import setup, Extension

# Define the C extension module
fast_math_module = Extension('fast_math', sources=['fast_math_ext.c'])

setup(
    name='FastMath',
    version='1.0',
    description='A C extension optimizing a heavy numeric loop.',
    ext_modules=[fast_math_module]
)
