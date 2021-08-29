import os  
import re
import shutil
import filecmp

from generateClassList import *

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
#print(cwd)

# folders to find classes
folders = ["code", "tools", "games"]

class_map = getClassList(cwd, folders)

#print(class_list)
#print(include_list)

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
    for key, class_def in class_map.items():
        file.write("REGISTER_CLASS_BY_NAME("+class_def.class_name+")\n")

with open(class_manager_includes_generated_file_path_tmp, "w") as file:
    for key, class_def in class_map.items():
        file.write("#include \""+class_def.include+"\"\n")


overwrite = True
if os.path.isfile(class_manager_generated_file_path) and os.path.isfile(class_manager_includes_generated_file_path):
    if filecmp.cmp(class_manager_generated_file_path, class_manager_generated_file_path_tmp, shallow=True) and filecmp.cmp(class_manager_includes_generated_file_path, class_manager_includes_generated_file_path_tmp, shallow=True):
        print(class_manager_generated + " and " + class_manager_includes_generated_file_path + " have no changes!")
        overwrite = False

if overwrite:
    print(class_manager_generated + " and " + class_manager_includes_generated_file_path + " have changes!")

    shutil.move(class_manager_generated_file_path_tmp, class_manager_generated_file_path)
    shutil.move(class_manager_includes_generated_file_path_tmp, class_manager_includes_generated_file_path)