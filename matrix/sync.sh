# Delete files if different
rsync -e 'ssh -p 22' -av --delete ../../mosaico-software root@pi:/root/working \
  --exclude='cross-compilers/' --exclude='x-tools' --exclude='CMakeFiles' --exclude='cmake_install.database' \
  --exclude='Makefile' --exclude='CMakeCache.txt' --exclude='venv' --exclude='CMakeFiles' --exclude='CMakeScripts' \
  --exclude='.git'
