# Unit tests

TestTarget(ut_gut_circular_buffer
    SOURCES
        ut_gut_circular_buffer.cpp
    LIBRARIES
        gut
    DEPENDS
        googletest
)

TestTarget(ut_gut_cleaner
    SOURCES
        ut_gut_cleaner.cpp
    LIBRARIES
        gut
    DEPENDS
        googletest
)

TestTarget(ut_gut_filequeue
    SOURCES
        ut_gut_filequeue.cpp
    LIBRARIES
        gut
    DEPENDS
        googletest
)

#TestTarget(ut_gut_filequeue_tail
#    SOURCES
#        ut_gut_filequeue_tail.cpp
#    LIBRARIES
#        gut
#    DEPENDS
#        googletest
#    DISABLE
#)

TestTarget(ut_gut_queue
    SOURCES
        ut_gut_queue.cpp
    LIBRARIES
        gut
    DEPENDS
        googletest
)
