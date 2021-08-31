import os  

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
#print(cwd)

os.system("electron .")

#cd ../../
#./binaries/test_integration_playground &


