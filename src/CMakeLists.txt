set(TARGET_NAME "${PROJECT_NAME}")

#boost
find_package(Boost 1.62.0 EXACT COMPONENTS system REQUIRED)
#add_compile_definitions(_HAS_AUTO_PTR_ETC=1)
#set(Boost_USE_STATIC_LIBS ON)

#qt
find_package(Qt5 REQUIRED COMPONENTS Core Gui)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(
    ${TARGET_NAME}
    main.cpp
    #NS.h
    VSError.h
    VSError.ipp
    VSError.cpp
    VSUtils.h
    #AbstractWindowPositioner.h
    #AbstractWindowPositioner.cpp
    VSContainer.h
    VSSequencedSet.h
    VSNotificationManager.h
    VSNotificationManager.cpp
    #SequencedSet.h
    #SequencedSet.ipp
    #OrderableSequencedSet.h
    #OrderableSequencedSet.ipp
    #AnyIterator.h
    #IExporterAsImages.h
)

target_include_directories(
	${TARGET_NAME}
	PRIVATE
	${Boost_INCLUDE_DIRS}
	#${Qt5Core_INCLUDE_DIRS}
	#${Qt5Gui_INCLUDE_DIRS}
)

target_link_libraries(
	${TARGET_NAME}
	PRIVATE
	${Boost_LIBRARIES}
	Qt5::Gui
	Qt5::Core
)
