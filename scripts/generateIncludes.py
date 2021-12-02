import os  
import re
import shutil

from generateClassList import *

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
#print(cwd)

# folders to find classes
folders = ["code", "tools", "games"]

class_map = getClassList(cwd, folders)

forward_map = {}
include_map = {}

generated_code_dirname = "generated-code/includes"

if os.path.isdir(generated_code_dirname):
    shutil.rmtree(generated_code_dirname)

os.mkdir(generated_code_dirname)

for folder in folders:
    path = cwd+"/"+folder
    for root,d_names,f_names in os.walk(path):
        for f in f_names:
            file_name = os.path.join(root, f)
            if ".hpp" in file_name and "Macros" not in file_name:
                with open(file_name, 'r') as file:
                    lines = file.readlines()
                    classes_defined_in_file = set()
                    for line in lines:
                        match = searchClassDefinition(line)
                        if match:
                            classes_defined_in_file.add(match.group(1))

                    if len(classes_defined_in_file) != 0:
                        include_map["core"] = ClassDef("core", "code/Core/Core.hpp")

                        for line in lines:
                            match_native_forward = re.search(r'\s*class\s*.*\s*;\s*', line)
                            if not match_native_forward:
                                for key, class_def in class_map.items():
                                    matchPtrOrRef = re.search(r'\s*[,\s]\s*' + class_def.class_name + '\s*[\*\&]\s*', line)
                                    if matchPtrOrRef and class_def.class_name not in classes_defined_in_file:
                                        print(file_name + ' class name found (pointer or ref): ' + matchPtrOrRef.group(0))
                                        forward_map[class_def.class_name] = class_def
                                        #include_map.pop(class_def.class_name, None) # if forward found, remove include
                                    else:
                                        matchClass = re.search(r'\s*[,\s]\s*' + class_def.class_name + '\s*', line)
                                        if matchClass and class_def.class_name not in classes_defined_in_file:
                                            print(file_name + ' class name found: ' + matchClass.group(0))
                                            #if class_def.class_name not in forward_map:
                                            # only include if there's no forward
                                            include_map[class_def.class_name] = class_def

                        file_base_name = os.path.basename(file_name).split(".")[0]
                        with open("generated-code/includes/"+file_base_name+"_includes.hpp", "w") as file:
                            for key, class_def in include_map.items():
                                file.write("#include \""+class_def.include+"\"\n")
                            for key, class_def in forward_map.items():
                                file.write("class "+class_def.class_name+";\n")

                            include_map.clear()
                            forward_map.clear()
