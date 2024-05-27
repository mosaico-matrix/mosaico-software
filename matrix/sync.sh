# Delete files if different
rsync -e 'ssh -p 22' -av --delete ../magic-square-software root@pi:/root/tesi/MagicSquare \
  --exclude='cross-compilers/' --exclude='x-tools' --exclude='CMakeFiles' --exclude='cmake_install.database' \
  --exclude='Makefile' --exclude='CMakeCache.txt' --exclude='venv' --exclude='CMakeFiles'
