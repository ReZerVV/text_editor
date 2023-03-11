cmake_minimum_required(VERSION 3.14)

# ---------------- PROJECT ---------------- #
project(te CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_FLAGS "-Wall -Werror")

file(GLOB SOURCES "source/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
include_directories("${PROJECT_SOURCE_DIR}/include")

# ---------------- ADD LIBRARIES ---------------- #
include(FindPkgConfig)
pkg_check_modules(SDL REQUIRED sdl2>=2.0.1)
#pkg_check_modules(SDL_image REQUIRED SDL_image)
#pkg_check_modules(SDL_ttf REQUIRED SDL_ttf)

include_directories( ${PROJECT_NAME}
  ${SDL_INCLUDE_DIRS}
#  ${SDL_IMAGE_INCLUDE_DIRS}
#  ${SDL_TTF_INCLUDE_DIRS}
  )
link_directories(${CMAKE_SOURCE_DIR}/libs/template_application)
include_directories("${PROJECT_SOURCE_DIR}/libs/template_application/include")

# ---------------- LINK ---------------- #
set(SDL_APP_LIBRARIES ${CMAKE_SOURCE_DIR}/build/libtemplate_sdl_application.a)
target_link_libraries(${PROJECT_NAME} 
  ${SDL_LIBRARIES}
  ${SDL_APP_LIBRARIES}
#  ${SDL_IMAGE_LIBRARIES}
#  ${SDL_TTF_LIBRARIES}
  )
