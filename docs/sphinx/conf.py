# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

import subprocess

# These are defaults set to use with read-the-docs but when generated
# automatically with CMake they will be set using CMake Variables

project = 'falcie'
copyright = '2022, James Calo'
author = 'James Calo'

# The full version, including alpha/beta/rc tags
release = '0.0.1'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [ "breathe"
]

#--------------------------------------------------------------------#
# These are hardcoded values for breathe used so that read-the-docs
# can operate corectly despite not having CMake.
# Therefore, when the documentation is generated automatically with 
# CMake these variables will be set from within CMake using the 
# Sphinx target 
#--------------------------------------------------------------------#
# read-the-docs_doxygen = 
breathe_projects = { project : "../build/doxygen/xml/" }
breathe_default_project = project




# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'alabaster'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']


######################################################################
## Read the docs config ##
######################################################################

def run_doxygen_on_rtd(path):
  doxygen_command = f"cd {path}; doxygen doxygen/Doxyfile"
  completed_process = subprocess.run(doxygen_command, shell=True,
                                     capture_output=True)
  ret = completed_process.returncode
  if ret != 0:
    sys.stderr.write(f"*** Doxygen execution failed with return code {ret} ***")
    sys.stderr.write(completed_process.stderr.decode())
    sys.stderr.write("*** ***")
    completed_process.check_returncode()

def generate_doxygen_xml_on_rtd(app):
  # Not -1000 as True is a string here
  if os.environ.get('READTHEDOCS', None) == 'True':
    run_doxygen_on_rtd("..")

def setup(app):
  app.connect("builder-inited", generate_doxygen_xml_on_rtd)
