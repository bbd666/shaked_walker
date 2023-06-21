To run the Visual Studio solution, this folder must contain:
/assimp-5.00 
/Walking_Humanoid_Shaked
/glew-2.1.0 
/glfw-3.3.2 
/glm-0.9.8 
/newton-3.14

driver Nvidia from https://www.nvidia.fr/Download/driverResults.aspx/155714/fr


Compile assimp 5.0.0
and copy in folder Walking_Humanoid_Shaked :

assimp-vc142-mt.dll
assimp-vc142-mtd.dll


Download Newton Dynamics: modeapp su github:https://github.com/MADEAPPS/newton-dynamics/archive/f22cf9d09f1665de0d8c5902b818fd558c3c011c.zip

name of the commit -> newton-dynamics-f22cf9d09f1665de0d8c5902b818fd558c3c011c 

Copy in this folder the newton dynamics folder downloaded from github.

Compile Newton dynmics sln (both ir release and debug mode) (VS 2015) and copy files

dContainers.dll
dContainers_d.dll
dCustomJoints.dll
dCustomJoints_d.dll
dNewton.dll
dNewton_d.dll
dScene.dll
dScene_d.dll
newton.dll
newton_d.dll

in folder Walking_Humanoid_Shaked 

Update also files in folder: Walking_Humanoid_Shaked\newtonPlugins

Open the Shaked_walker_VS2019.sln file

Additional Include Directories:

..\newton-3.14\sdk\dContainers
..\newton-3.14\sdk\dModel
..\newton-3.14\sdk\dMath
..\newton-3.14\sdk\dgNewton
..\newton-3.14\sdk\dCustomJoints
..\newton-3.14\sdk\dgMeshUtil
.\stb_image"
..\newton-3.14\sdk\dgCore
.\System
..\glew-2.1.0\include\GL
..\glfw-3.3\include\GLFW
..\glm

Project properties->linker->input->Additional Dependencies and add the following lines

"wsock32.lib" 
"opengl32.lib" 
"..\glfw-3.3\src\Debug\glfw3.lib" 
"..\glew-2.1.0\lib\Debug\x64\glew32sd.lib" 
"..\newton-3.14\sdk\projects\visualStudio_2015_dll\x64\newton\Debug\newton_d.lib" 
"..\newton-3.14\sdk\projects\visualStudio_2015_dll\x64\dCustomJoints\Debug\dCustomJoints_d.lib" 
"..\newton-3.14\sdk\projects\visualStudio_2015_dll\x64\dMath\Debug\dMath_d.lib" 
"..\newton-3.14\sdk\projects\visualStudio_2015_dll\x64\dModel\Debug\dModel_d.lib" 
"..\newton-3.14\sdk\projects\visualStudio_2015_dll\x64\dContainers\Debug\dContainers_d.lib" 
"..\newton-3.14\sdk\projects\visualStudio_2015_dll\x64\dVehicle\Debug\dVehicle_d.lib"

enjoy