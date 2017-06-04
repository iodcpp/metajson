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
        elif re.match("namespace\s*iod\s*", line):
            output.write("namespace metajson")
        else:
            output.write(line)

if __name__ == "__main__":

    if os.path.exists(sys.argv[1]):
        print(sys.argv[1], "already exists")
        exit(1)

    output=open(sys.argv[1], 'w')
    
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

    # cd build_dir
    os.chdir(build_dir)

    # Install
    subprocess.check_call(["cmake", src_dir, "-DCMAKE_INSTALL_PREFIX=" + install_dir, "-DCMAKE_CXX_COMPILER=clang++"])
    subprocess.check_call(["make", "install", "-j4"])

    # Generate single file header.
    process_file(join(install_dir, "include/iod/metajson/metajson.hh"), processed, output)

    print("metajson single header", sys.argv[1], "generated.")
    # Cleanup
    shutil.rmtree(tmp_dir)
