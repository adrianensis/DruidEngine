import os

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
#print(cwd) 

for_each_file = "generated-code/for_each.generated.hpp"

max_for_each = 1000

if not os.path.isfile(for_each_file):
    with open(for_each_file, "w") as file:
        file.write("#define EXPAND(x) x\n")

        nargs_str = ""
        for n in range(0, max_for_each):
            nargs_str = nargs_str + "x" + str(n+1) + ","

        file.write("#define DEF_AUX_NARGS(" + nargs_str + "VAL, ... )   VAL\n")
        #file.write("#define NARGS(...)          EXPAND(DEF_AUX_NARGS(__VA_ARGS__,10,9,8,7,6,5,4,3,2,1,0))\n")
        
        file.write("#define FE0_1(what,x)       EXPAND(what(x))\n")

        for n in range(1, max_for_each):
            file.write("#define FE0_" + str(n+1) + "(what,x, ...)  EXPAND(what(x)FE0_" + str(n) + "(what, __VA_ARGS__))\n")

        nargs_str = ""
        for n in range(0, max_for_each):
            nargs_str = nargs_str + "FE0_" + str(max_for_each - n) + ","
        
        file.write("#define REPEAT0_(...)       EXPAND(DEF_AUX_NARGS(__VA_ARGS__," + nargs_str + "0))\n")
        
        file.write("#define FOR_EACH(what,...)  EXPAND(REPEAT0_(__VA_ARGS__)(what,__VA_ARGS__))\n")