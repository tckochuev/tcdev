add_compile_definitions(_HAS_AUTO_PTR_ETC=1)

add_executable(${PROJECT_NAME}
    main.cpp
    VSError.h
    VSIImageRange.h
    VSIImageRange.ipp
    VSImageProviderRange.h
    VSImageProviderRange.ipp
    VSUtils.h
    AbstractWindowPositioner.h
    AbstractWindowPositioner.cpp
    VSContainer.h
    ValueProperties.h
    SequencedSet.h
    SequencedSet.ipp
    OrderableSequencedSet.h
    OrderableSequencedSet.ipp
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE .
    ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib/boost
)
