configure preset="dev":
    cmake --preset {{ preset }} --fresh

build-web:
    cmake --build build/web

build-dev:
    cmake --build build/dev

build-release:
    cmake --build build/release

run-dev: build-dev
    ./build/dev/game

watch-dev:
    watchexec -w src "just build-dev"

serve: build-web
    python3 -m http.server -d build/web
