import os

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
#print(cwd) 

def generateCode():
    generated_code_dirname = "generated-code"
    generated_code_path = os.path.join(cwd, generated_code_dirname)

    if not os.path.isdir(generated_code_path):
        os.mkdir(generated_code_dirname)
        
    # -B : prevents from writing .pyc or .pyo files
    os.system('python3 -B ./scripts/generateInstanceByName.py')