INTRODUCTION
============

 **LXCManager** is an open-source project which aims to manage *LinuX
 Containers* on a server with an easy-to-use web interface.

BUILD
=====

DEPENDANCIES
------------

 This project comes with dependancies (*libboost* or *cppwebsocket*...). You
 will first need to build and install this dependancies in you build
 environment system. But, in order to not impact you own system, these
 dependancies will be store in the project's root and the build system will
 link them automatically.
 **To launch the dependancies building, use the following command:**

    42sh$ make all

 The first time you build the project, the build system will download the last
 source version of each dependancy, build them and install then in the *lib*
 subdirectory. You can update them by using the following command:

    42sh$ make update-repos

CONFIGURATION
-------------

 This project uses the **Kconfig** configuration system to tune the project.
 You can change the project name, the version number or the default log level
 for example. You can use the following command to use the *ncurse* interface
 to change the options:

    42sh$ make meuconfig

 You also can use the default configuration wich comes with the project:

    42sh$ make defconfig

COMPILING
---------

 Now, the project's dependancies are installed in the build system. You can use
 the usual *Makefile targets* to work with the project:

    42sh$ make all

DOCUMENTATION
=============

 The project comes with **Doxygen** documentation. To generate it, use the
 following command:

    42sh$ make doc

 The documentation will be readable in **HTML** format in the documentation
 directory: *$(PROJECTDIR)/doc/html/index.html*

TEST
====

 To test the project, the plugins will probably need to be dynamicaly linked to
 the project's dependancy libraries (for example the plugin *communication*
 needs the boost\_system library available) then as this library comes from the
 project's dependancies, you may need to add the project's library path to your
 environment system. For example, the following command should works:

    42sh$ LD_LIBRARY_PATH=${PWD}/lib/lib ./build/lxcmanager --help

