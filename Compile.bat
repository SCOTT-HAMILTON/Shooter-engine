@echo off
echo Enter SFML location (this folder needs to have a lib and a include subfolder) :
set /P SFMLdir=""

set lib_static=-lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lwinmm -lopengl32 -lgdi32 -lfreetype
set lib_dynamic=-lsfml-graphics -lsfml-window -lsfml-system
set param_compile=-I%CD% -I %SFMLdir%\include -std=c++11
set param_link=-L%SFMLdir%\lib -L%CD%\deps %lib_dynamic% -o shooter.exe

echo g++ -c main.cpp %param_compile% 
g++ -c main.cpp %param_compile%
echo g++ -c Drawer.cpp %param_compile%
g++ -c Drawer.cpp %param_compile%
echo g++ main.o Drawer.o %param_link%
g++ main.o Drawer.o %param_link%
echo Executable created !
