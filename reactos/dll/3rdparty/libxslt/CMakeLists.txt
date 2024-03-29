
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/libs/libxslt
                    ${REACTOS_SOURCE_DIR}/lib/3rdparty/libwin-iconv)

add_definitions(
    -DHAVE_CONFIG_H
    -DWIN32
    -D_WINDOWS
    -D_MBCS
    -DHAVE_STAT
    -DHAVE_WIN32_THREADS
    -D_REENTRANT
    -D_WINSOCKAPI_
    -DLIBXML_STATIC)

list(APPEND SOURCE
    attributes.c
    attrvt.c
    documents.c
    extensions.c
    extra.c
    functions.c
    imports.c
    keys.c
    namespaces.c
    numbers.c
    pattern.c
    preproc.c
    security.c
    templates.c
    transform.c
    variables.c
    xslt.c
    xsltlocale.c
    xsltutils.c
    precomp.h)

add_library(libxslt SHARED ${SOURCE})
set_module_type(libxslt win32dll)
target_link_libraries(libxslt libxml2 iconv-static zlib)
add_importlibs(libxslt msvcrt ws2_32 kernel32)

if(MSVC)
    add_importlibs(libxslt ntdll)
endif()

add_pch(libxslt precomp.h SOURCE)
add_cd_file(TARGET libxslt DESTINATION reactos/system32 FOR all)

if(NOT MSVC)
    add_target_compile_flags(libxslt "-Wno-pointer-sign -Wno-unused-function")
    if(NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
        add_target_compile_flags(libxslt "-Wno-unused-but-set-variable")
    endif()
endif()
