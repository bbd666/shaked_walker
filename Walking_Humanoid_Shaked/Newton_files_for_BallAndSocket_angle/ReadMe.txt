Copy 

dCustomBallAndSocket.cpp
dCustomBallAndSocket.h

in newton-3.14\sdk\dCustomJoints.

Open build.sln in newton-3.14\sdk\projects\visualStudio_2015_dll

Build the solution  in release and debug mode.

copy the following dlls in shaked_walker\DG_Tutorial_Newton:

dContainers.dll
dContainers_d.dll
dCustomJoints.dll
dCustomJoints_d.dll
newton.dll
newton_d.dll
dNewton.dll
dNewton_d.dll
dScene.dll
dScene_d.dll


Now you can use a dCustomBallAndSocket joint in shaked walker.

details:

Added 
GetJointAngle() and GetPinAxis() methods and m_curJointAngle() inside dCustomAndSocket.cpp and the respective header file. 
The method are the same used in dCustomHinge.cpp 


Optional
copy also dlls in shaked_walker\DG_Tutorial_Newton\newtonPlugins

dgNewtonAvx_d.dll
dgNewtonAvx2_d.dll
dgNewtonSse4.2_d.dll

dgNewtonAvx.dll
dgNewtonAvx2.dll
dgNewtonSse4.2.dll