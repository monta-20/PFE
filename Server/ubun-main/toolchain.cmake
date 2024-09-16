# Définir le nom du système cible
set(CMAKE_SYSTEM_NAME Linux)

# Définir l'architecture cible
set(CMAKE_SYSTEM_PROCESSOR arm)

# Définir le chemin du sysroot (optionnel)
set(CMAKE_SYSROOT /home/devel/rasp-pi-rootfs)

# Définir le chemin des outils de cross-compilation
set(tools /home/devel/gcc-4.7-linaro-rpi-gnueabihf)

# Définir les compilateurs C et C++ pour la cross-compilation
set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)

# Configurer la recherche des chemins de l'outil de cross-compilation
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
