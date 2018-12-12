@echo OFF
cd Bin
cmake .. -G "Visual Studio 15"
if NOT ["%errorlevel%"]==["0"] pause