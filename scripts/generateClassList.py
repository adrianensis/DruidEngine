import os  
import re

class ClassDef:
  def __init__(self, class_name, include):
    self.class_name = class_name
    self.include = include

def searchClassDefinition(text):
    return re.search(r'^\s*CLASS\s*\(\s*(\w+)\s*,\s*\w+\s*\)\s*', text)

def getClassList(currentDir, folders):

    class_map = {}

    for folder in folders:
        path = currentDir+"/"+folder
        for root,d_names,f_names in os.walk(path):
            for f in f_names:
                file_name = os.path.join(root, f)
                if ".hpp" in file_name and "Macros" not in file_name:
                    with open(file_name, 'r') as file:
                        lines = file.readlines()
                        for line in lines:
                            match = searchClassDefinition(line)
                            if match:
                                class_name = match.group(1)
                                relative_include = file_name.replace(currentDir+"/", '')
                                
                                class_map[class_name] = ClassDef(class_name, relative_include)
    
    return class_map