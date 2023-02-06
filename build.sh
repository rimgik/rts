# build and run script

if g++ -std=c++17 -O2 ./src/*.cpp -I./include -I/opt/homebrew/include -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lbox2d -o ./bin/game; then
./bin/game;
fi

