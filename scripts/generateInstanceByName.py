import os  
import re
import shutil
import filecmp
import pathlib

from generateClassList import *

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
#print(cwd)

# folders to find classes
foldersMap = {}

keyFolder_code = "code"

foldersMap[keyFolder_code] = "code"

# add folders to foldersMap
foldersToExplore = ["tools", "games"]

for folder in foldersToExplore:
    current_path = os.path.join(cwd, folder)
    directory_contents = os.listdir(current_path)
    for item in directory_contents:
        if os.path.isdir(os.path.join(current_path, item)):
            foldersMap[item] = os.path.join(folder, item) # foldersMap["Editor"] = "tools/Editor"

generated_code_dirname = "generated-code"
generated_code_dirname_tmp = os.path.join(generated_code_dirname, "tmp")

generated_code_path = os.path.join(cwd, generated_code_dirname)
generated_code_path_tmp = os.path.join(generated_code_path, "tmp")

if os.path.isdir(generated_code_path):
    if os.path.isdir(generated_code_path_tmp):
        shutil.rmtree(generated_code_dirname_tmp)

os.mkdir(generated_code_dirname_tmp)

for keyFolder, folder in foldersMap.items():
    pathlib.Path(os.path.join(generated_code_dirname_tmp, folder)).mkdir(parents=True, exist_ok=True)

for keyFolder, folder in foldersMap.items():

    class_map = getClassList(cwd, [folder])

    class_manager_generated = os.path.join(folder, "generated") # .replace("/", ".")
    class_manager_includes_generated = os.path.join(folder, "includes.generated") # .replace("/", ".")

    class_manager_generated_file_path = os.path.join(generated_code_path, class_manager_generated)
    class_manager_generated_file_path_tmp = os.path.join(generated_code_path_tmp, class_manager_generated)

    class_manager_generated_class = class_manager_generated_file_path + ".class.hpp"
    class_manager_generated_class_tmp = class_manager_generated_file_path_tmp + ".class.hpp"

    class_manager_includes_generated_file_path = os.path.join(generated_code_path, class_manager_includes_generated)
    class_manager_includes_generated_file_path_tmp = os.path.join(generated_code_path_tmp, class_manager_includes_generated)

    with open(class_manager_generated_file_path_tmp, "w") as file:
        for _, class_def in class_map.items():
            file.write("REGISTER_CLASS_BY_NAME("+class_def.class_name+")\n")

    with open(class_manager_includes_generated_file_path_tmp, "w") as file:
        for _, class_def in class_map.items():
            file.write("#include \""+class_def.include+"\"\n")

    if keyFolder != keyFolder_code:
        with open(class_manager_generated_class_tmp, "w") as file:

            classManagerName = "ModuleClassesRegister"

            file.write("#pragma once\n")
            
            # code dependencies
            file.write("#include \""+os.path.join(generated_code_dirname, os.path.join(foldersMap[keyFolder_code], "includes.generated"))+"\"\n")
            
            relative_include = class_manager_includes_generated_file_path.replace(cwd+"/", '')
            file.write("#include \""+relative_include+"\"\n")

            file.write("class " + classManagerName +"\n")
            file.write("{\n")
            file.write("public:\n")
            file.write(classManagerName +"()\n")
            file.write("{\n")

            # code dependencies
            file.write("#include \""+os.path.join(generated_code_dirname, os.path.join(foldersMap[keyFolder_code], "generated"))+"\"\n")

            relative_include = class_manager_generated_file_path.replace(cwd+"/", '')
            file.write("#include \""+relative_include+"\"\n")

            file.write("}\n")
            file.write("};\n")
            file.write("\n")

    overwrite = True
    if os.path.isfile(class_manager_generated_file_path) and os.path.isfile(class_manager_includes_generated_file_path):
        if filecmp.cmp(class_manager_generated_file_path, class_manager_generated_file_path_tmp, shallow=True) and filecmp.cmp(class_manager_includes_generated_file_path, class_manager_includes_generated_file_path_tmp, shallow=True):
            print(class_manager_generated + " and " + class_manager_includes_generated_file_path + " have no changes!")
            overwrite = False

    if overwrite:
        print(class_manager_generated + " and " + class_manager_includes_generated_file_path + " have changes!")

        pathlib.Path(os.path.join(generated_code_dirname, folder)).mkdir(parents=True, exist_ok=True)

        shutil.move(class_manager_generated_file_path_tmp, class_manager_generated_file_path, copy_function = shutil.copytree)
        shutil.move(class_manager_includes_generated_file_path_tmp, class_manager_includes_generated_file_path, copy_function = shutil.copytree)
        
        if keyFolder != keyFolder_code:
            shutil.move(class_manager_generated_class_tmp, class_manager_generated_class, copy_function = shutil.copytree)