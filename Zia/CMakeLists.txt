CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(Zia CXX)

# Where the binaries will be
SET(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)


INCLUDE_DIRECTORIES(
	../Submodule_Zia/
)

# Files used in all ports
FILE(
        GLOB_RECURSE
        SOURCE_FILES

        CachedData.h
        CacheException.h
		CacheManager.h
		CacheSerializer.h
        DataBase.hpp
        ISock.hpp
		NTPClock.h
        Server.hpp
        SockException.hpp
        ProtocolException.hpp
        SockInclude.hpp
        Struct.hpp
        UnixSock.hpp
        WinSock.hpp
        TCPAcceptor.hpp
        TCPStream.hpp
		ThreadPool.hpp
		Thread.hpp
		ModuleLoader.h
        OpenSSLModule.hpp

        main.cpp
        CachedData.cpp
		CacheManager.cpp
		CacheSerializer.cpp
        NTPClock.cpp
		UnixSock.cpp
        WinSock.cpp
        Server.cpp
        TCPAcceptor.cpp
        TCPStream.cpp
		ThreadPool.cpp
		Thread.cpp
		ModuleLoader.cpp
)

FILE(
	GLOB_RECURSE
	HEADER_FACTORY

	HeaderFactory.h
	HeaderFactory.cpp	
)


FILE(
	GLOB_RECURSE
	SOURCE_API

	../Submodule_Zia/*.cpp
)

FILE(
	GLOB_RECURSE
	HEADER_API

	../Submodule_Zia/*.hh
    ../Submodule_Zia/*.hpp
)


LIST(REMOVE_ITEM SOURCE_API "${CMAKE_CURRENT_SOURCE_DIR}/../Submodule_Zia/main.cpp")

SOURCE_GROUP("Header Files\\API_HEADER" FILES ${HEADER_API})
SOURCE_GROUP("Source Files\\API_SOURCE" FILES ${SOURCE_API})

# Remove Unix files from Windows build
IF(WIN32)
    LIST(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/UnixSock.cpp")
    LIST(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/UnixSock.hpp")
ENDIF()

# Remove Windows files from Unix build
IF(UNIX)
    LIST(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/WinSock.cpp")
    LIST(REMOVE_ITEM SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/WinSock.hpp")
ENDIF()

ADD_EXECUTABLE(
        Zia
        ${SOURCE_FILES}
		${SOURCE_API}
		${HEADER_API}
		${HEADER_FACTORY}
)

IF(WIN32)
    TARGET_LINK_LIBRARIES(
            Zia
            Ws2_32
    )
ENDIF()

IF(UNIX)
    TARGET_LINK_LIBRARIES(
            Zia
            pthread
    )
ENDIF()

# Enable C++11
TARGET_COMPILE_FEATURES(Zia PRIVATE cxx_range_for)
