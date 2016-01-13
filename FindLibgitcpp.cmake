find_path(
  LIBGITCPP_INCLUDE_DIR NAMES Repository.h Commit.h
  PATH_SUFFIXES libgitcpp/include
  PATHS
  /home/tim/Projects/
  /var/lib
  /opt
  /opt/local
  HINTS
  ${PC_LIBGITCPP_INCLUDEDIR}
)

message("Libgitcpp include dir: " ${LIBGITCPP_INCLUDE_DIR})
include_directories(${LIBGITCPP_INCLUDE_DIR})

mark_as_advanced(LIBGITCPP_INCLUDE_DIR)
