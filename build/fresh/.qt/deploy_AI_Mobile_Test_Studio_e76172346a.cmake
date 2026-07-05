include("E:/Project/AI_Mobile_Test_Studio/build/fresh/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/AI_Mobile_Test_Studio-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE E:/Project/AI_Mobile_Test_Studio/build/fresh/AI_Mobile_Test_Studio.exe
    GENERATE_QT_CONF
)
