#! /usr/bin/python3

import tempfile
import subprocess
import os,re,sys,shutil

from os.path import join

def process_file(f, processed, output):

    if f in processed:
        return
    processed.append(f)
    for line in open(f, "r"):
        m = re.match("^#include\s*<(iod/.*)>.*$", line)
        if m:
            process_file(join(install_dir, "include", m.group(1)), processed, output)
        elif re.match("#pragma once", line):
            pass
        elif re.match("namespace\s*iod(\s*)", line):
            m2 = re.match("namespace\s*iod(\s*)", line)
            output.append("namespace metajson" + m2.group(1))
        else:
            output.append(line)

if __name__ == "__main__":

    if os.path.exists(sys.argv[1]):
        print(sys.argv[1], "already exists")
        exit(1)
    

    output_file=open(sys.argv[1], 'w')

        # create temp directory.
    tmp_dir=tempfile.mkdtemp()
    src_dir=join(tmp_dir, "src")
    build_dir=join(tmp_dir, "build")
    install_dir=join(tmp_dir, "install")

    os.mkdir(src_dir)
    os.mkdir(build_dir)
    os.mkdir(install_dir)

    processed=[]

    # git clone recursive iod
    subprocess.check_call(["git", "clone", "--recursive", "https://github.com/iodcpp/iod", src_dir])
    # subprocess.check_call(["git", "clone", "/home/matt/iod", src_dir])
    # subprocess.check_call(["cp", "-R", "/home/matt/iod", src_dir])

    # cd build_dir
    os.chdir(build_dir)

    # Install
    subprocess.check_call(["cmake", src_dir, "-DCMAKE_INSTALL_PREFIX=" + install_dir, "-DCMAKE_CXX_COMPILER=clang++"])
    subprocess.check_call(["make", "install", "-j4"])

    # Generate single file header.
    lines=[]
    process_file(join(install_dir, "include/iod/metajson/metajson.hh"), processed, lines)

    body=[]
    includes=[]
    for line in lines:
        m = re.match("^\s*#include.*$", line)
        if m:
            includes.append(line)
        else:
            body.append(line)

    output_file.write("// Single header version of iod::metajson [1].\n")
    output_file.write("// This file is generated, do not edit it directly.\n")
    output_file.write("// [1] https://github.com/iodcpp/metajson\n\n")
    output_file.write("#pragma once\n\n")
    for l in includes:
        output_file.write(l)
    output_file.write("\n\n")
    for l in body:
        output_file.write(l)
    output_file.close()
    
    print("metajson single header", sys.argv[1], "generated.")
    # Cleanup
    shutil.rmtree(tmp_dir)
