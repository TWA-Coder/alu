from setuptools import setup, Extension

# Define the extension module
vibration_module = Extension(
    'vibration',               # The name of the module we will import in Python
    sources=['vibration.c'],   # The C source file
    extra_compile_args=['-O3'] # Optional: Optimize the C code for performance
)

# Run the setup script
setup(
    name='vibration_analytics',
    version='1.0',
    description='A high-performance C extension for industrial vibration arrays.',
    ext_modules=[vibration_module]
)
