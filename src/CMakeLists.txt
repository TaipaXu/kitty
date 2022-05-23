find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)
find_package(Threads)

set( CURL_LIBRARY ${CMAKE_SOURCE_DIR}/3rd/curl/build/lib )
set( CURL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/3rd/curl/include )

include_directories( ${CURL_INCLUDE_DIR} )
link_directories( ${CURL_LIBRARY} )

configure_file(./config.hpp.in ./config.hpp)

set(TARGET kitty)
set(SOURCES
        main.cpp
        core.hpp
        core.cpp
        widgets/mainWindow.hpp
        widgets/mainWindow.cpp
        widgets/nav.hpp
        widgets/nav.cpp
        widgets/navbutton.hpp
        widgets/navbutton.cpp
        widgets/manager.hpp
        widgets/manager.cpp
        widgets/managerProject.hpp
        widgets/managerProject.cpp
        widgets/projectDialog.hpp
        widgets/projectDialog.cpp
        widgets/projects.hpp
        widgets/projects.cpp
        widgets/explorer.hpp
        widgets/explorer.cpp
        widgets/tabbar.hpp
        widgets/tabbar.cpp
        widgets/tab.hpp
        widgets/tab.cpp
        widgets/api.hpp
        widgets/api.cpp
        widgets/request.hpp
        widgets/request.cpp
        widgets/requestTable.hpp
        widgets/paramTable.hpp
        widgets/paramTable.cpp
        widgets/requestParams.hpp
        widgets/requestParams.cpp
        widgets/requestHeaders.hpp
        widgets/requestHeaders.cpp
        widgets/headerTable.hpp
        widgets/headerTable.cpp
        widgets/requestBody.hpp
        widgets/requestBody.cpp
        widgets/formDataTable.hpp
        widgets/formDataTable.cpp
        widgets/xWwwFormUrlEncodedTable.hpp
        widgets/xWwwFormUrlEncodedTable.cpp
        widgets/filePicker.hpp
        widgets/filePicker.cpp
        widgets/requestSettings.hpp
        widgets/requestSettings.cpp
        widgets/groupDialog.hpp
        widgets/groupDialog.cpp
        widgets/response.hpp
        widgets/response.cpp
        widgets/responseSummary.hpp
        widgets/responseSummary.cpp
        widgets/responseTable.cpp
        widgets/responseHeaders.hpp
        widgets/responseHeaders.cpp
        widgets/responseBody.hpp
        widgets/responseBody.cpp
        widgets/histories.hpp
        widgets/histories.cpp
        widgets/settings.hpp
        widgets/settings.cpp
        widgets/apiSettings.hpp
        widgets/apiSettings.cpp
        models/language.hpp
        models/language.cpp
        models/project.hpp
        models/project.cpp
        models/group.hpp
        models/group.cpp
        models/api.hpp
        models/api.cpp
        models/method.hpp
        models/method.cpp
        models/param.hpp
        models/param.cpp
        models/header.hpp
        models/header.cpp
        models/body.hpp
        models/body.cpp
        models/formData.hpp
        models/formData.cpp
        models/xWwwFormUrlEncoded.hpp
        models/xWwwFormUrlEncoded.cpp
        models/raw.hpp
        models/raw.cpp
        models/binary.hpp
        models/binary.cpp
        models/response.hpp
        models/response.cpp
        qmodels/api.hpp
        qmodels/api.cpp
        qmodels/item.hpp
        qmodels/item.cpp
        qmodels/table.hpp
        qmodels/param.hpp
        qmodels/param.cpp
        qmodels/header.hpp
        qmodels/header.cpp
        qmodels/formData.hpp
        qmodels/formData.cpp
        qmodels/responseHeader.hpp
        qmodels/responseHeader.cpp
        qmodels/xWwwFormUrlEncoded.hpp
        qmodels/xWwwFormUrlEncoded.cpp
        qmodels/formDataDelegate.hpp
        qmodels/formDataDelegate.cpp
        persistence/persistence.hpp
        persistence/persistence.cpp
        persistence/project.hpp
        persistence/project.cpp
        persistence/settings.hpp
        persistence/settings.cpp
        persistence/apiSettings.hpp
        persistence/apiSettings.cpp
        events/bus.hpp
        events/bus.cpp
        utils/utils.hpp
        utils/utils.cpp
        network/network.hpp
        network/network.cpp
        assets.qrc
)

add_executable(
    ${TARGET}
    ${SOURCES}
)

target_link_libraries(${TARGET}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    ${CMAKE_THREAD_LIBS_INIT}
    curl)
