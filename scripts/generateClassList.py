import os  
import re
import shutil
import filecmp

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
#print(cwd) 

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
                    #print("looking for CLASS " + file_name)
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

# print(class_list)
# print(includes)

generated_code_dirname = "generated-code"
generated_code_dirname_tmp = os.path.join(generated_code_dirname, "tmp")
generated_code_path = os.path.join(cwd, generated_code_dirname)
generated_code_path_tmp = os.path.join(generated_code_path, "tmp")
class_manager_generated = "ClassManager.generated"
class_manager_includes_generated = "ClassManager.includes.generated"
class_manager_generated_file_path = os.path.join(generated_code_path, class_manager_generated)
class_manager_generated_file_path_tmp = os.path.join(generated_code_path_tmp, class_manager_generated)
class_manager_includes_generated_file_path = os.path.join(generated_code_path, class_manager_includes_generated)
class_manager_includes_generated_file_path_tmp = os.path.join(generated_code_path_tmp, class_manager_includes_generated)

if os.path.isdir(generated_code_path):
    if os.path.isdir(generated_code_path_tmp):
        shutil.rmtree(generated_code_dirname_tmp)
else:
    os.mkdir(generated_code_dirname)

os.mkdir(generated_code_dirname_tmp)

with open(class_manager_generated_file_path_tmp, "w") as file:
    for class_name in class_list:
        file.write("REGISTER_CLASS_BY_NAME("+class_name+")\n")

with open(class_manager_includes_generated_file_path_tmp, "w") as file:
    for include_path in includes:
        file.write("#include \""+include_path+"\"\n")


overwrite = True
if os.path.isfile(class_manager_generated_file_path) and os.path.isfile(class_manager_includes_generated_file_path):
    if filecmp.cmp(class_manager_generated_file_path, class_manager_generated_file_path_tmp, shallow=True) and filecmp.cmp(class_manager_includes_generated_file_path, class_manager_includes_generated_file_path_tmp, shallow=True):
        print(class_manager_generated + " and " + class_manager_includes_generated_file_path + " have no changes!")
        overwrite = False

if overwrite:
    print(class_manager_generated + " and " + class_manager_includes_generated_file_path + " have changes!")

    shutil.move(class_manager_generated_file_path_tmp, class_manager_generated_file_path)
    shutil.move(class_manager_includes_generated_file_path_tmp, class_manager_includes_generated_file_path)