rmdir /s /q lib
rmdir /s /q build
git clone https://github.com/OpenEtherCATsociety/SOEM.git lib
::xcopy /s /i "CMakeLists.txt" "lib"
copy CMakeLists.txt lib\.
cd lib
mkdir build
cd build
cmake .. -G "NMake Makefiles"
nmake install
cd ..
cd ..