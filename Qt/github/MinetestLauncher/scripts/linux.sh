#!/bin/bash

# Linux instance creator
# $1 - Action to take
# $2 - Instance name
# $3 - Version to create the instance with
TARGET=~/.minetest/instances/$2

libs () {
	if ! ls -R /usr/include |grep irrlicht
	then
		if [ $(which pacman) -neq "pacman not found" ]
		then
			sudo pacman -S irrlicht
		elif [ $(which apt-get) -neq "apt-get not found" ]
		then
			sudo apt-get install libirrlicht-dev
		else
			echo "Cannot automatically install irrlicht libraries. Please do so yourself before installing irrlicht"
		fi
	fi
}

# Minetest installer function
install () {
	MINETEST_LINK=https://codeload.github.com/minetest/minetest/tar.gz/$VERSION
	GAME_LINK=https://codeload.github.com/minetest/minetest_game/tar.gz/$VERSION

	echo "Creating instance directory"
	mkdir -p $TARGET
	cd $TARGET

	echo "Downloading instance from $MINETEST_LINK"
	curl -o src.tar.gz $MINETEST_LINK
	tar -xvf src.tar.gz
	rm src.tar.gz

	mv minetest-$VERSION/* .
	rm -r minetest-$VERSION

	echo "Compiling Minetest"
	cmake . && make -j$(($(cat /proc/cpuinfo |grep processor |wc -l) * 2))

	echo "Downloading Minetest game from $GAME_LINK"
	cd games
	curl -o game.tar.gz $GAME_LINK
	tar -xvf game.tar.gz
	mv minetest_game* minetest_game
	rm game.tar.gz
}

case $1 in
	create)
		VERSION=$3
		libs
		install
                cd ..
                ./bin/minetest
		;;
	play)
		cd $TARGET
		./bin/minetest
		;;
esac

