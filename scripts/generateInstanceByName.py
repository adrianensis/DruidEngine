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
foldersMap = {}

foldersMap["code"] = "code"
foldersMap["Editor"] = "tools/Editor"
foldersMap["EditorElectron"] = "tools/EditorElectron"
foldersMap["games"] = "games"

for keyFolder, folder in foldersMap.items():

    class_map = getClassList(cwd, [folder])

    generated_code_dirname = "generated-code"
    generated_code_dirname_tmp = os.path.join(generated_code_dirname, "tmp")

    generated_code_path = os.path.join(cwd, generated_code_dirname)
    generated_code_path_tmp = os.path.join(generated_code_path, "tmp")

    class_manager_generated = keyFolder.replace("/", ".") + ".generated"
    class_manager_includes_generated = keyFolder.replace("/", ".") + ".includes.generated"

    class_manager_generated_file_path = os.path.join(generated_code_path, class_manager_generated)
    class_manager_generated_file_path_tmp = os.path.join(generated_code_path_tmp, class_manager_generated)

    class_manager_generated_class = class_manager_generated_file_path + ".class.hpp"
    class_manager_generated_class_tmp = class_manager_generated_file_path_tmp + ".class.hpp"

    class_manager_includes_generated_file_path = os.path.join(generated_code_path, class_manager_includes_generated)
    class_manager_includes_generated_file_path_tmp = os.path.join(generated_code_path_tmp, class_manager_includes_generated)

    if os.path.isdir(generated_code_path):
        if os.path.isdir(generated_code_path_tmp):
            shutil.rmtree(generated_code_dirname_tmp)

    os.mkdir(generated_code_dirname_tmp)

    with open(class_manager_generated_file_path_tmp, "w") as file:
        for _, class_def in class_map.items():
            file.write("REGISTER_CLASS_BY_NAME("+class_def.class_name+")\n")

    with open(class_manager_includes_generated_file_path_tmp, "w") as file:
        for _, class_def in class_map.items():
            file.write("#include \""+class_def.include+"\"\n")

    with open(class_manager_generated_class_tmp, "w") as file:

        classManagerName = "ClassManager_"+ keyFolder.replace("/", "_")

        file.write("#pragma once\n")
        relative_include = class_manager_includes_generated_file_path.replace(cwd+"/", '')
        file.write("#include \""+relative_include+"\"\n")
        file.write("class " + classManagerName +"\n")
        file.write("{\n")
        file.write("public:\n")
        file.write(classManagerName +"()")
        file.write("{\n")
        relative_include = class_manager_generated_file_path.replace(cwd+"/", '')
        file.write("#include \""+relative_include+"\"\n")
        file.write("}\n")
        file.write("};\n")
        file.write("#define REGISTER_CLASSES_BY_NAME() " + classManagerName + "();")
        file.write("\n")

    overwrite = True
    if os.path.isfile(class_manager_generated_file_path) and os.path.isfile(class_manager_includes_generated_file_path):
        if filecmp.cmp(class_manager_generated_file_path, class_manager_generated_file_path_tmp, shallow=True) and filecmp.cmp(class_manager_includes_generated_file_path, class_manager_includes_generated_file_path_tmp, shallow=True):
            print(class_manager_generated + " and " + class_manager_includes_generated_file_path + " have no changes!")
            overwrite = False

    if overwrite:
        print(class_manager_generated + " and " + class_manager_includes_generated_file_path + " have changes!")

        shutil.move(class_manager_generated_file_path_tmp, class_manager_generated_file_path)
        shutil.move(class_manager_includes_generated_file_path_tmp, class_manager_includes_generated_file_path)
        shutil.move(class_manager_generated_class_tmp, class_manager_generated_class)