include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if (NOT DEFINED CMAKE_INSTALL_DOCDIR)
    set(CMAKE_INSTALL_DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME}")
endif ()

set(AVERRA_PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraGlobal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraVersion.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraTheme.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraThemeManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraFieldDefinition.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraValidationResult.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraFormModel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraIcon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraButton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraLineEdit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraComboBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraNumberBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraDateEdit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraCard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraInfoBar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraNavigationPanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraSwitch.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraRadioButton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraTag.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraProgressBar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraCheckBox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraSlider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraSegmentedControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraTabs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraSectionHeader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraDivider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraTextArea.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraEmptyState.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraStatisticCard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraLoadingRing.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraSkeleton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraToast.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraDialogManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraToastManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraBadgeButton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraSearchBar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraToolbar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraRibbonBar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraFilterChipGroup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraPagination.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraPropertyItem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraPropertyGrid.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraFormSection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraSettingsPage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraPropertyEditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraInspectorPanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraDataTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraTreeView.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraTreeTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraDock.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraDrawer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraContextMenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra/AverraNumberRangeBox.h
)

set(AVERRA_PRIVATE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/core/AverraTheme.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/AverraThemeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/AverraFieldDefinition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/AverraValidationResult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/AverraFormModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/AverraIcon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/AverraStyleHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraButton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraLineEdit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraComboBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraNumberBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraDateEdit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraCard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraInfoBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraNavigationPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraSwitch.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraRadioButton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraTag.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraProgressBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraCheckBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraSlider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraSegmentedControl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraTabs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraSectionHeader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraDivider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraTextArea.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraEmptyState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraStatisticCard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraLoadingRing.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraSkeleton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraToast.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraDialogManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraToastManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraBadgeButton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraSearchBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraToolbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraRibbonBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraFilterChipGroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraPagination.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraPropertyItem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraPropertyGrid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraFormSection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraSettingsPage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraPropertyEditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraInspectorPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraDataTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraTreeView.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraTreeTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraDock.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraDrawer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraContextMenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgets/AverraNumberRangeBox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../resources/averra_resources.qrc
)

add_library(AverraWidgets SHARED
    ${AVERRA_PUBLIC_HEADERS}
    ${AVERRA_PRIVATE_SOURCES}
)

add_library(Averra::Widgets ALIAS AverraWidgets)

target_compile_definitions(AverraWidgets PRIVATE AVERRA_BUILD_LIBRARY)

target_link_libraries(AverraWidgets
    PUBLIC
        Qt5::Widgets
)

target_include_directories(AverraWidgets
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(AverraWidgets PROPERTIES
    OUTPUT_NAME AverraWidgets
    EXPORT_NAME Widgets
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/AverraWidgetsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/AverraWidgetsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/AverraWidgetsConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AverraWidgets
)

install(TARGETS AverraWidgets
    EXPORT AverraWidgetsTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/Averra
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT AverraWidgetsTargets
    FILE AverraWidgetsTargets.cmake
    NAMESPACE Averra::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AverraWidgets
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/AverraWidgetsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/AverraWidgetsConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AverraWidgets
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../CHANGELOG.md
    ${CMAKE_CURRENT_SOURCE_DIR}/../README.md
    ${CMAKE_CURRENT_SOURCE_DIR}/../docs/SCENARIO_GUIDE.md
    ${CMAKE_CURRENT_SOURCE_DIR}/../docs/ROADMAP.md
    ${CMAKE_CURRENT_SOURCE_DIR}/../docs/RELEASE.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)
