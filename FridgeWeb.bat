:: Build tool for Web platform
:: It mirrors the premake configurations
:: Premake supports emscriptem build target but the setup is tricky and this will do for now
:: TODO: use premake...

@echo off

setlocal enabledelayedexpansion enableextensions

:: Location of Emscripten SDK (modify this)
set EMSDK="C:\...\emsdk\"
if not exist %EMSDK% (
	echo Error: No Emscripten SDK folder found!
	goto :fail
)

:: Comment if you have troubles with emsdk
set EMSDK_QUIET=1

set WORKINGDIR=%CD%

set PROJECT=%1

:: Run specified action
if "%2"=="build" goto :build
if "%2"=="run" goto :run
if "%2"=="deploy" goto :deploy
if "%2"=="clean" goto :clean
if "%2"=="fullclean" goto :fullclean
echo Error: Incorrect Input
goto :fail

:build
	:: Configure path variables
	echo Configuring Emscripten SDK path...
	cd %EMSDK%
	call emsdk_env.bat

	:: Compile dependencies
	echo Compiling dependencies...

	:: Glad
	cd %WORKINGDIR%\Eis\vendor\GLAD
	if not exist ".\bin-int\Release-web\Glad\glad.o" (
		echo Compiling GLAD...
		if not exist ".\bin\Release-web\Glad" mkdir .\bin\Release-web\Glad
		if not exist ".\bin-int\Release-web\Glad" mkdir .\bin-int\Release-web\Glad
		call emcc .\src\glad.c -I .\include -c -O3 -o .\bin-int\Release-web\Glad\glad.o
	) else echo Found glad.o

	:: ImGui
	cd %WORKINGDIR%\Eis\vendor\ImGui
	if not exist ".\bin-int\Release-web\ImGui\imgui.o" (
		echo Compiling ImGui...
		if not exist ".\bin\Release-web\ImGui" mkdir .\bin\Release-web\ImGui
		if not exist ".\bin-int\Release-web\ImGui" mkdir .\bin-int\Release-web\ImGui

		call em++ .\imgui.cpp .\imgui_draw.cpp .\imgui_tables.cpp .\imgui_widgets.cpp .\backends\imgui_impl_glfw.cpp .\backends\imgui_impl_opengl3.cpp -I .\ -c -O3 -w --use-port=contrib.glfw3
		call em++ .\imgui.o .\imgui_draw.o .\imgui_tables.o .\imgui_widgets.o .\imgui_impl_glfw.o .\imgui_impl_opengl3.o -r -O3 -o .\bin-int\Release-web\ImGui\imgui.o

		del *.o
	) else echo Found imgui.o

	:: ImPlot
	cd %WORKINGDIR%\Eis\vendor\implot
	if not exist ".\bin-int\Release-web\ImPlot\implot.o" (
		echo Compiling ImPlot...
		if not exist ".\bin\Release-web\ImPlot" mkdir .\bin\Release-web\ImPlot
		if not exist ".\bin-int\Release-web\ImPlot" mkdir .\bin-int\Release-web\ImPlot

		call em++ .\implot.cpp .\implot_items.cpp -I ..\imgui -c -w -O3 --use-port=contrib.glfw3
		call em++ .\implot.o .\implot_items.o -r -O3 -o .\bin-int\Release-web\ImPlot\implot.o

		del *.o
	) else echo Found implot.o

	:: GLM
	cd %WORKINGDIR%\Eis\vendor\glm
	if not exist ".\bin-int\Release-web\glm\glm.o" (
		echo Compiling glm...
		if not exist ".\bin\Release-web\glm" mkdir .\bin\Release-web\glm
		if not exist ".\bin-int\Release-web\glm" mkdir .\bin-int\Release-web\glm

		call em++ .\glm\detail\glm.cpp -I .\ -c -O3 -o .\bin-int\Release-web\glm\glm.o
	) else echo Found glm.o

	:: Compile Eis
	cd %WORKINGDIR%\Eis
	if not exist ".\bin-int\Release-web\Eis\eis.o" (
		echo Compiling Eis...		
		if not exist ".\bin\Release-web\Eis" mkdir .\bin\Release-web\Eis
		if not exist ".\bin-int\Release-web\Eis" mkdir .\bin-int\Release-web\Eis

		:: Find Eis files
		set CPP=
		for /f %%x in ('dir /s /b /a-d .\src\*.cpp') do set CPP=!CPP! %%x
		:: Find STB files
		for /f "delims=" %%x in ('dir /s /b /a-d .\vendor\*.cpp ^| findstr /i \\stb_') do set CPP=!CPP! %%x
		:: Add Spdlog files
		for /f %%x in ('dir /s /b /a-d .\vendor\spdlog\src\*.cpp') do set CPP=!CPP! %%x
		set CPP=!CPP:~1!
		set CPP=!CPP:%CD%=.!

		call em++ !CPP! -I.\src -I.\vendor\Glad\include -I.\vendor\imgui -I.\vendor\implot -I.\vendor\glm -I.\vendor\spdlog\include -I.\vendor\stb_image -I.\vendor\stb_image_write -I.\vendor\stb_image_resize -DGLFW_INCLUDE_NONE -DEIS_DEBUG -DSPDLOG_COMPILED_LIB -c -sTOTAL_STACK=512mb -O3 -w --use-port=contrib.glfw3

		set OBJ=
		for /f %%x in ('dir /b /a-d .\*.o') do set OBJ=!OBJ! %%x
		set OBJ=!OBJ:~1!

		call em++ !OBJ! -r -sTOTAL_STACK=512mb -O3 -o .\bin-int\Release-web\Eis\eis.o

		del *.o
	) else echo Found eis.o

	:: Compile project
	cd %WORKINGDIR%
	echo Compiling %PROJECT%...
	if not exist ".\bin\Release-web\%PROJECT%" mkdir .\bin\Release-web\%PROJECT%
	if not exist ".\bin-int\Release-web\%PROJECT%" mkdir .\bin-int\Release-web\%PROJECT%

	set CPP=
	for /f %%x in ('dir /s /b /a-d .\%PROJECT%\*.cpp') do set CPP=!CPP! %%x
	set CPP=%CPP:~1%

	call em++ %CPP% -I .\%PROJECT%\src -I .\Eis\src -I .\Eis\vendor\spdlog\include -I .\Eis\vendor\imgui -I .\Eis\vendor\implot -I .\Eis\vendor\glm -DEIS_DEBUG -sTOTAL_STACK=512mb -c -w -O3 --use-port=contrib.glfw3

	set OBJ=
	for /f %%x in ('dir /b /a-d .\*.o') do set OBJ=!OBJ! %%x
	set OBJ=!OBJ:~1!

	call em++ %OBJ% -r -O3 -o .\bin-int\Release-web\%PROJECT%\%PROJECT%.o
	del *.o

	:: Link
	echo Linking project...
	set OBJ=.\bin-int\Release-web\%PROJECT%\%PROJECT%.o .\Eis\bin-int\Release-web\Eis\eis.o .\Eis\vendor\GLAD\bin-int\Release-web\Glad\glad.o .\Eis\vendor\glm\bin-int\Release-web\glm\glm.o .\Eis\vendor\imgui\bin-int\Release-web\ImGui\imgui.o .\Eis\vendor\implot\bin-int\Release-web\ImPlot\implot.o

	call em++ %OBJ% -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -O3 -o .\bin\Release-web\%PROJECT%\index.html --use-port=contrib.glfw3 --preload-file .\%PROJECT%\assetsWeb\@\assets

	copy /y .\template.html .\bin\Release-web\%PROJECT%\index.html >nul

	echo Build completed
	goto :success

:run
	cd %EMSDK%
	call emsdk_env.bat
	cd %WORKINGDIR%
	if not exist ".\bin\Release-web\%PROJECT%\index.html" (
		echo Error: No index.html file found!
		goto :fail
	)
	emrun .\bin\Release-web\%PROJECT%\index.html
	goto :success

:deploy
	if exist ".\bin\Release-web\%PROJECT%\index.html" (
		if not exist ".\pages" mkdir pages
		copy /y bin\Release-web\%PROJECT%\* .\pages >NUL
		goto :success
	) else (
		echo Error: No index.html file found!
		goto :fail
	)

:fullclean
	if exist ".\Eis\vendor\glad\bin\Release-web" rmdir /s /q .\Eis\vendor\glad\bin\Release-web
	if exist ".\Eis\vendor\glad\bin-int\Release-web" rmdir /s /q .\Eis\vendor\glad\bin-int\Release-web
	if exist ".\Eis\vendor\imgui\bin\Release-web" rmdir /s /q .\Eis\vendor\imgui\bin\Release-web
	if exist ".\Eis\vendor\imgui\bin-int\Release-web" rmdir /s /q .\Eis\vendor\imgui\bin-int\Release-web
	if exist ".\Eis\vendor\implot\bin\Release-web" rmdir /s /q .\Eis\vendor\implot\bin\Release-web
	if exist ".\Eis\vendor\implot\bin-int\Release-web" rmdir /s /q .\Eis\vendor\implot\bin-int\Release-web
	if exist ".\Eis\vendor\glm\bin\Release-web" rmdir /s /q .\Eis\vendor\glm\bin\Release-web
	if exist ".\Eis\vendor\glm\bin-int\Release-web" rmdir /s /q .\Eis\vendor\glm\bin-int\Release-web
	if exist ".\Eis\bin\Release-web" rmdir /s /q .\Eis\bin\Release-web
	if exist ".\Eis\bin-int\Release-web" rmdir /s /q .\Eis\bin-int\Release-web
	goto :clean

:clean
	if exist ".\bin\Release-web\%PROJECT%" rmdir /s /q .\bin\Release-web\%PROJECT%
	goto :success

:success
	echo Fridge success
	goto :leave

:fail
	echo Fridge failure
	goto :leave

:leave