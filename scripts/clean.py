import os  
import shutil

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
#print(cwd)

def remove_folder(folder):
    if os.path.isdir(folder):
        shutil.rmtree(folder)

remove_folder("binaries")
remove_folder("build")
remove_folder("generated-code")