import os

def clear_files():
    for root, dirs, files in os.walk("."):
        for file in files :
            suf = file.split('.')[-1]
            if ( suf == "c" or suf == "h" or suf == "cpp" or \
                 suf == "makefile" or suf == "ld" or \
                 suf == "json" or suf == "S" or suf == "s" ):
                print(root + "/" + file)
                clear_file(root + "/" + file)

def clear_file(file):
    count = 0 # 为中间换行计数
    lines_out = []
    fin = open(file, "r")
    lines_in = fin.readlines()
    for line in lines_in:
        line = line.rstrip()
        if not ( line ):
            count += 1
            continue
        while ( count ):
            line = "\n" + line
            count -= 1
        line += "\n"
        lines_out.append(line)
    fin.close()

    fout = open(file, "w")
    for line in lines_out:
        fout.write(line)
    fout.close()

if ( __name__ == "__main__" ):
    clear_files()
