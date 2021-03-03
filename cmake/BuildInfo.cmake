cmake_minimum_required(VERSION 3.14)

# These values indicate "something went wrong"
set(PRODUCT_BRANCH_ID   "0" CACHE STRING "Branch ID")
set(PRODUCT_BUILD_ID    "0" CACHE STRING "Build ID")
set(PRODUCT_BUILD_TYPE  "0" CACHE STRING "Build Type")

if(PRODUCT_EMBED_BUILD_INFO)
    find_package(Git QUIET)
    if(Git_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --always --dirty --exclude '*'
            OUTPUT_VARIABLE GIT_REV
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        execute_process(
            COMMAND git diff --exit-code
            OUTPUT_VARIABLE GIT_DIFF_CONTENTS
            RESULT_VARIABLE GIT_DIFF
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        execute_process(
            COMMAND git describe --exact-match --tags
            OUTPUT_VARIABLE GIT_TAG
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        string(REGEX REPLACE "[\r\n]" " " GIT_TAG "${GIT_TAG}")

        if(NOT GIT_TAG)
            set(GIT_TAG ${GIT_REV})
        endif()
    endif()
endif()

if(NOT GIT_REV)
    set(GIT_REV "untracked")
endif()
if(NOT GIT_TAG)
    set(GIT_TAG ${GIT_REV})
endif()

if(PRODUCT_EMBED_BUILD_TIME)
    # Include the hash of the diff to ensure that non-commit changes trigger a build-time update.
    string(SHA256 CURRENT_DIFF_HASH "${GIT_DIFF_CONTENTS}")

    if(WIN32)
        execute_process(
            COMMAND powershell -Command "Get-Date -UFormat +%Y-%m-%d"
            OUTPUT_VARIABLE BUILD_DATE
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        execute_process(
            COMMAND powershell -Command "Get-Date -UFormat +%H:%M%Z"
            OUTPUT_VARIABLE BUILD_TIME
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        # Force UTC offset to four digits instead of two
        string(REGEX REPLACE [[(-|\+)([0-9][0-9])$]] [[\1\200]] BUILD_TIME "${BUILD_TIME}")
    else()
        execute_process(
            COMMAND date +%Y-%m-%d
            OUTPUT_VARIABLE BUILD_DATE
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        execute_process(
            COMMAND date +%H:%M%z
            OUTPUT_VARIABLE BUILD_TIME
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()
else()
    set(CURRENT_DIFF_HASH "untracked")
    set(BUILD_DATE "untracked")
    set(BUILD_TIME "untracked")
endif()

# Write the result to a temporary file and compare only the parts of the build info generated by git.
# If those changed, then copy the new file over. Otherwise, what you get is a build, then install
# will trigger re-linking all executables because time keeps on slippin', slippin' (into the future...)
configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/hsBuildInfo.inl.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/hsBuildInfo_new.inl"
    ESCAPE_QUOTES @ONLY
)

if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/hsBuildInfo.inl")
    set(BUILD_DIRTY TRUE)
else()
    file(READ "${CMAKE_CURRENT_BINARY_DIR}/hsBuildInfo.inl" _OLD_BUILDINFO)
    file(READ "${CMAKE_CURRENT_BINARY_DIR}/hsBuildInfo_new.inl" _NEW_BUILDINFO)
    # remove the contents of the literal if it's volatile, eg a date/time
    string(REGEX REPLACE [[(VOLATILE_[^=]*)[^;]*]] [[\1= "untracked"]] _OLD_BUILDINFO "${_OLD_BUILDINFO}")
    string(REGEX REPLACE [[(VOLATILE_[^=]*)[^;]*]] [[\1= "untracked"]] _NEW_BUILDINFO "${_NEW_BUILDINFO}")
    string(COMPARE NOTEQUAL "${_NEW_BUILDINFO}" "${_OLD_BUILDINFO}" BUILD_DIRTY)
endif()

if(BUILD_DIRTY)
    message("Updated hsBuildInfo.inl.")
    file(RENAME "${CMAKE_CURRENT_BINARY_DIR}/hsBuildInfo_new.inl" "${CMAKE_CURRENT_BINARY_DIR}/hsBuildInfo.inl")
else()
    message("No change to hsBuildInfo.inl.")
    file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/hsBuildInfo_new.inl")
endif()
