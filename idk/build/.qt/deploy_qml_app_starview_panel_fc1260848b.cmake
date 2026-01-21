include("/home/kirby/Programming/StarShell/idk/build/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/starview-panel-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative")

qt6_deploy_qml_imports(TARGET starview-panel PLUGINS_FOUND plugins_found)
qt6_deploy_runtime_dependencies(
    EXECUTABLE "/home/kirby/Programming/StarShell/idk/build/starview-panel"
    ADDITIONAL_MODULES ${plugins_found}
    GENERATE_QT_CONF
)