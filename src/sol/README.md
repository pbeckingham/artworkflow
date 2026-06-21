Upgrading sol2:

$ git clone git@github.com:ThePhD/sol2.git
$ cd sol2
$ cmake .
$ make
$ cd single
$ python3 ./single/single.py --input $PWD/include --output $PWD/sol.hpp
cp sol.hpp config.hpp forward.hpp ~/artworkflow/src/sol/
