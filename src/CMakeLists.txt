set(TARGET_NAME "${PROJECT_NAME}")

#boost
add_compile_definitions(_HAS_AUTO_PTR_ETC=1)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.62.0 REQUIRED filesystem serialization regex)

add_executable(
    ${TARGET_NAME}
    main.cpp
    NS.h
    Error.h
    Error.ipp
    Error.cpp
    Utils.h
    AbstractWindowPositioner.h
    AbstractWindowPositioner.cpp
    Container.h
    SequencedSet.h
    SequencedSet.ipp
    OrderableSequencedSet.h
    OrderableSequencedSet.ipp
    AnyIterator.h
    IExporterAsImages.h
)

target_include_directories(
	${TARGET_NAME}
	PRIVATE
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(
	${TARGET_NAME} 
	PRIVATE
	${Boost_LIBRARIES}
)
