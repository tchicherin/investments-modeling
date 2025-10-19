# src/CMakeLists.txt
add_compile_definitions(PROJECT_ROOT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

set(SOURCE_FILES
        main.cpp # create a minimal main.cpp when ready; not required for headers-only compile
)

# Create the executable (you will add .cpp files; here is target skeleton)
add_executable(InvestSim
        ${SOURCE_FILES}
        # add your .cpp sources here, e.g. RandomEngine.cpp Market.cpp Simulator.cpp ...
)

target_include_directories(InvestSim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(InvestSim PRIVATE Qt6::Widgets Qt6::Charts)

# for IDE / CLion convenience: mark headers as sources so they show up
set(HEADER_FILES
        RandomEngine.h
        Market.h
        Investment.h
        InvestmentItem.h
        Portfolio.h
        Fund.h
        Simulator.h
        investments/Deposit.h
        investments/Stock.h
)

# add headers for the project tree
target_sources(InvestSim PRIVATE ${HEADER_FILES})

# If you want to enable warnings and sanitizers locally, add here
if(MSVC)
    target_compile_options(InvestSim PRIVATE /W4 /permissive-)
else()
    target_compile_options(InvestSim PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
endif()
