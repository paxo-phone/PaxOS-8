# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/gabriel/Bureau/paxos_8/libcurl/src/libcurl"
  "/home/gabriel/Bureau/paxos_8/libcurl/src/libcurl-build"
  "/home/gabriel/Bureau/paxos_8/libcurl"
  "/home/gabriel/Bureau/paxos_8/libcurl/tmp"
  "/home/gabriel/Bureau/paxos_8/libcurl/src/libcurl-stamp"
  "/home/gabriel/Bureau/paxos_8/libcurl/src"
  "/home/gabriel/Bureau/paxos_8/libcurl/src/libcurl-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/gabriel/Bureau/paxos_8/libcurl/src/libcurl-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/gabriel/Bureau/paxos_8/libcurl/src/libcurl-stamp${cfgdir}") # cfgdir has leading slash
endif()
