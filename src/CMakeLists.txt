# Add executable targets
########################
ADD_EXECUTABLE(infod DeviceInfo.c InfoDaemon.c)
# Add library targets
#####################
FIND_LIBRARY(LIB_AWA libawa.so ${STAGING_DIR}/usr/lib)
TARGET_LINK_LIBRARIES(infod ${LIB_AWA})
# Add install targets
######################
INSTALL(TARGETS infod RUNTIME DESTINATION bin)