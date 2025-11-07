cmake_minimum_required(VERSION 3.21)
project(PortfolioSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)

add_executable(PortfolioSim
    app/main.cpp
    app/MainWindow.h app/MainWindow.cpp
    app/GameController.h app/GameController.cpp
    app/Market.h app/Market.cpp
    app/Portfolio.h app/Portfolio.cpp
    app/Investment.h app/Investment.cpp
    app/DepositInvestment.h app/DepositInvestment.cpp
    app/StockInvestment.h app/StockInvestment.cpp
    app/InvestmentFund.h app/InvestmentFund.cpp
    app/Utils.h
        app/Utils.cpp
        app/CurrencyInvestment.h
        app/CurrencyInvestment.cpp
        app/BondInvestment.h
        app/BondInvestment.cpp
        app/MetalInvestment.h
        app/MetalInvestment.cpp
)

target_link_libraries(PortfolioSim PRIVATE Qt6::Widgets)

if(APPLE)
    set_target_properties(PortfolioSim PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.portfoliosim
    )
endif()
