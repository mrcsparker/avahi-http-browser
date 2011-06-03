cmake_minimum_required(VERSION 2.8)

project("avahi-http-browser" C)

find_package(PkgConfig)
pkg_check_modules(GTK2 REQUIRED gtk+-3.0)
pkg_check_modules(AVAHI REQUIRED avahi-glib avahi-client)
pkg_check_modules(AVAHI_UI REQUIRED avahi-ui-gtk3)

include_directories(${GTK2_INCLUDE_DIRS} ${AVAHI_INCLUDE_DIRS} ${AVAHI_UI_INCLUDE_DIRS})
link_directories(${GTK2_LIBRARY_DIRS} ${AVAHI_LIBRARY_DIRS} ${AVAHI_UI_LIBRARY_DIRS})
add_definitions(${GTK2_CFLAGS} ${AVAHI_CFLAGS} ${AVAHI_UI_CFLAGS})

add_executable(avahi-http-browser src/main.c)

target_link_libraries(avahi-http-browser ${GTK2_LIBRARIES} ${AVAHI_LIBRARIES} ${AVAHI_UI_LIBRARIES})
