import os  
import shutil
import platform

from zipfile import ZipFile

import tarfile

if not platform.python_version().startswith('3'):
    print("Please use python3 to run this script")
    exit()

def extract(file_path, destinty_path):
    print("Extracting " + file_path)
    if file_path.endswith("zip"):
        with ZipFile(file_path) as zipObj:
            zipObj.extractall(destinty_path)

    if file_path.endswith("tar.xz"):
        with tarfile.open(name=file_path, mode='r:xz') as tarObj:
            tarObj.extractall(destinty_path)

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
#print(cwd)

destiny="dependencies"
origin_path_base="scripts/dependencies-zip"

if os.path.isdir(destiny):
    shutil.rmtree(destiny)

os.mkdir(destiny)

system_name = platform.system()
system_info = str(platform.uname())

print(system_name)
print(system_info)

if system_name == "Linux" or system_name == "Linux2":
    # linux
    if "Ubuntu" in system_info:
        os.system("sudo apt-get -y update")
        os.system("sudo apt-get -y install build-essential wget unzip cmake clang cmake-data libglu1-mesa-dev freeglut3-dev mesa-common-dev xorg-dev doxygen graphviz")

elif system_name == "Darwin":
    # OS X
    pass
elif system_name == "Win32":
    # Windows...
    pass
# elif ANDROID:
#     # Windows...
#     pass
# elif IOS:
#     # Windows...
#     pass

# ------------------------------------------------------------------------

# GLFW
extract(os.path.join(origin_path_base, "glfw-3.2.1.zip"), destiny)

# ------------------------------------------------------------------------

# glad
extract(os.path.join(origin_path_base, "glad.zip"), destiny + "/glad")

# ------------------------------------------------------------------------

# JSON https://github.com/nlohmann/json
extract(os.path.join(origin_path_base, "json-3.9.1.zip"), destiny)

# ------------------------------------------------------------------------

# libpng https://sourceforge.net/projects/libpng/files/latest/download
extract(os.path.join(origin_path_base, "libpng-1.6.37.tar.xz"), destiny)

# ------------------------------------------------------------------------