import os  
import re
import shutil

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
print(cwd) 

os.chdir(cwd)

# folders to find classes

folders = ["code", "tools"]

class_list = []
includes = []

for folder in folders:
    path = cwd+"/"+folder
    for root,d_names,f_names in os.walk(path):
        for f in f_names:
            file_name = os.path.join(root, f)
            if ".hpp" in file_name and "Macros" not in file_name:
                with open(file_name, 'r') as file:
                    print("looking for CLASS " + file_name)
                    lines = file.readlines()
                    for line in lines:
                        match = re.search(r'^\s*CLASS\s*\(\s*(\w+)\s*,\s*\w+\s*\)\s*', line)
                        if match:
                            relative_include = file_name.replace(cwd+"/", '')
                            if relative_include not in includes:
                                includes.append(relative_include)

                            class_name = match.group(1)
                            if class_name not in class_list:
                                class_list.append(class_name)

print(class_list)
print(includes)

generated_code_dirname = "generated-code"

shutil.rmtree(generated_code_dirname)
os.mkdir(generated_code_dirname)

generated_code_path = os.path.join(cwd, generated_code_dirname)

with open(os.path.join(generated_code_path, "ClassManager.generated"), "w") as class_list_file:
    for class_name in class_list:
        class_list_file.write("REGISTER_CLASS_BY_NAME("+class_name+")\n")

with open(os.path.join(generated_code_path, "ClassManager.includes.generated"), "w") as includes_list_file:
    for include_path in includes:
        includes_list_file.write("#include \""+include_path+"\"\n")

# for root,d_names,f_names in os.walk(path):
#     for f in f_names:
#         file_name = os.path.join(root, f)
#         if ".hpp" in file_name:
#             with open(file_name, 'r') as file:
#                 print("looking for includes " + file_name)
#                 data = file.read()

#                 includes = []
#                 forwards = []

#                 for c in class_list:
#                     matchClass = re.search(r''+ c , data) # + '^(\s*\*)'
#                     if matchClass:
#                         print(matchClass.group())
#                         forwards.append(c)

#                     # matchPointer = re.search(r''+ c + '\s*\*', data)
#                     # if matchPointer:
#                     #     print(matchPointer.group())
#                     #     forwards.append(c)

#                 print("includes: " + str(includes))
#                 print("forwards: " + str(forwards))