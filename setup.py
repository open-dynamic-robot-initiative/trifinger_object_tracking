from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

d = generate_distutils_setup(
        packages=['trifinger_object_tracking'],
        package_dir={'': 'python'}
)

setup(**d)
