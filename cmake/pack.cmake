set(CPACK_BUNDLE_NAME "argparse")
set(CPACK_PACKAGE_CONTACT "arwed.mett@googlemail.com")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://gitea.metthub.de/Pfeifenjoy/argparse")
set(CPACK_PACKAGE_CHECKSUM "SHA512")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")

if(GIT_CURRENT_VERSION MATCHES "[0-9]+.[0-9]+.[0-9]+")
	set(CPACK_PACKAGE_FILE_NAME
		"argparse-${CMAKE_SYSTEM_NAME}-${GIT_CURRENT_VERSION}")
else()
	set(CPACK_PACKAGE_FILE_NAME
		"argparse-${CMAKE_SYSTEM_NAME}-${GIT_BRANCH}-${GIT_COMMIT_HASH}")
endif()

message(STATUS "Package name: ${CPACK_PACKAGE_FILE_NAME}")

if(APPLE)
	set(CPACK_GENERATOR "TGZ;ZIP;productbuild")
elseif(UNIX)
	set(CPACK_GENERATOR "TGZ;ZIP;RPM;DEB")
elseif(WIN32)
	set(CPACK_GENERATOR "ZIP;NSIS;NSIS64")
else()
	set(CPACK_GENERATOR "TGZ;ZIP")
endif()

include(CPack)
