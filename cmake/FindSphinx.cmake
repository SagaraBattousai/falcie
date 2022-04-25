# Hints get searched before the system paths.
# They should only be set by some source of knowledge, 
# location of other files, etc and not populated with "guesses" or default locations.
set(_SPHINX_ROOT_HINTS
  ${SPHINX_ROOT_DIR}
  ENV SPHINX_ROOT_DIR
  ${Python_ROOT_DIR}
  )

# Paths get searched after system locations.
# This is the place to put default locations.
set(_SPHINX_ROOT_PATHS
  ${Python_ROOT_DIR}/Scripts
  )


find_program(SPHINX_EXECUTABLE
  NAMES sphinx-build
  HINTS ${_SPHINX_ROOT_HINTS}
  PATHS ${_SPHINX_ROOT_PATHS}
  PATH_SUFFIXES Scripts # May need to remove but could be good
  DOC "Path to sphinx-build executable")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Sphinx
  DEFAULT_MSG
  SPHINX_EXECUTABLE)


