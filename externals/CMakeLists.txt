if (BUILD_TESTS)
    ExternalTarget(googlebenchmark
        VERSION "1.9.5"
        URL     https://github.com/google/benchmark/archive/refs/tags/v1.9.5.tar.gz
        URL_MD5 "12c6c0c228fc07106c62634222bd2541"
        CONFIGURE_COMMAND
            cmake -DBENCHMARK_ENABLE_GTEST_TESTS=OFF
                -DBENCHMARK_DOWNLOAD_DEPENDENCIES=OFF
                -DCMAKE_BUILD_TYPE=Release
                --install-prefix ${EXTERNALS_PREFIX}/googlebenchmark/install ./
        BUILD_COMMAND
            cmake --build ./ --config Release && cmake --install ./
        INCLUDE_DIR
            ${EXTERNALS_PREFIX}/googlebenchmark/install/include
        INSTALL_DIR
            ${EXTERNALS_PREFIX}/googlebenchmark/install
        LIBRARIES
            libbenchmark.a
            libbenchmark_main.a
    )

    ExternalTarget(googletest
        VERSION "1.17.0"
        URL     https://github.com/google/googletest/archive/refs/tags/v1.17.0.tar.gz
        URL_MD5 "b6f100bc2a5853a48046aa168ececf84"
        CONFIGURE_COMMAND
            cmake --install-prefix ${EXTERNALS_PREFIX}/googletest/install ./
        BUILD_COMMAND
            cmake --build ./ && cmake --install ./
        INCLUDE_DIR
            ${EXTERNALS_PREFIX}/googletest/install/include
        INSTALL_DIR
            ${EXTERNALS_PREFIX}/googletest/install
        LIBRARIES
            libgmock.a
            libgmock_main.a
            libgtest.a
            libgtest_main.a
    )
endif()
