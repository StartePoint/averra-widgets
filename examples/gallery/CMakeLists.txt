add_executable(AverraGallery
    main.cpp
    GalleryWindow.h
    GalleryWindow.cpp
)

target_link_libraries(AverraGallery
    PRIVATE
        AverraWidgets
        Qt5::Widgets
)

set_target_properties(AverraGallery PROPERTIES
    WIN32_EXECUTABLE TRUE
)
