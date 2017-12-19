

IF(WIN32)
	ADD_DEFINITIONS(-DSTK_EXPORTS)
	set (FILETOOLS WIN_FileTools.cpp)
	TARGET_LINK_LIBRARIES(stk Shlwapi.lib Dbghelp.lib PsApi.lib)
ELSE(WIN32)
	set (FILETOOLS UNIX_FileTools.cpp)
ENDIF(WIN32)


ADD_LIBRARY(stk SHARED
../../include/stk/Box2.h
../../include/stk/Box3.h
../../include/stk/Color.h
../../include/stk/FileTools.h
../../include/stk/InertialMatrix.h
../../include/stk/Macro.h
../../include/stk/Mark.h
../../include/stk/Math.h
../../include/stk/Matrix.h
../../include/stk/Mecanics.h
../../include/stk/PID.h
../../include/stk/Quaternion.h
../../include/stk/STK.h
../../include/stk/STL.h
../../include/stk/Torser.h
../../include/stk/Transform.h
../../include/stk/Transform2D.h
../../include/stk/Unit.h
../../include/stk/Vector2.h
../../include/stk/Vector3.h
	Box2.cpp
	Box3.cpp
	Color.cpp
	FileTools.cpp
	${FILETOOLS}
	InertialMatrix.cpp
	Mark.cpp
	Math.cpp
	Matrix.cpp
	Mecanics.cpp
	PID.cpp
	Quaternion.cpp
	STL.cpp
	StringTools.cpp
	Torser.cpp
	Transform.cpp
	Transform2D.cpp
	Unit.cpp
	Vector2.cpp
	Vector3.cpp
)

install (TARGETS stk DESTINATION lib)
install (DIRECTORY ../../include/stk DESTINATION include)
