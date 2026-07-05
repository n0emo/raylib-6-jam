configure preset="dev":
    cmake --preset {{ preset }} --fresh

build:
    cmake --build build
