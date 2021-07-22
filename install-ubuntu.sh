#!/bin/bash
#
# install-ubuntu.sh is the installation script for TSGL on Linux Ubuntu.
# Last updated: 06/30/26
# 
# -SUBJECT TO CHANGE-
################################################################

if [[ $1 ]]
then
        PREFIX=$1
        echo Install location $PREFIX
else
        echo Install location /usr
        PREFIX=/usr
fi
echo "Installing TSGL..."

#Determine the OpenGL version (has to be 3.0 or higher)
#(Use glxinfo, available in the mesa-utils package)
echo 
echo "Checking OpenGL version (must be 3.0 or higher)..."

sudo apt-get -y update
sudo apt-get -y install mesa-utils

GLVersInfo=$(glxinfo | grep OpenGL)

#http://stackoverflow.com/questions/18147884/shell-variable-in-a-grep-regex
#Get a string containing the version number.
GLVersString=$(echo "$GLVersInfo" | grep "OpenGL version string: ")

#http://stackoverflow.com/questions/7516455/sed-extract-version-number-from-string-only-version-without-other-numbers
#http://superuser.com/questions/363865/how-to-extract-a-version-number-using-sed
#Get the version number from the version string.
GLVersNum=$(echo "$GLVersString" | sed 's/[^0-9.]*\([0-9.]*\).*/\1/')

#http://tldp.org/LDP/abs/html/comparison-ops.html
#Check if the version is less than the threshold
#if [ "$GLVersNum" \< "3.0" ]
#then
#	echo "Your version of GL is: $GLVersNum."
#	echo "You need at least OpenGL version 3.0 or greater."
#	echo "Please update your drivers in order to continue."
#	echo "Try the following command to update your drivers:"
#	echo
#	echo "sudo ubuntu-drivers autoinstall"
#	echo  
#	echo "Abort."
#	exit 1
#else
#	echo "OpenGL version is sufficient to continue."
#fi

echo "Checking for g++..."
echo

#check if g++ is installed
#g corresponds to g++ (not allowed to have ++ in a variable name)g++

echo "Checking g++ version"
echo "You need at least g++ 4.8 in order to continue"

gVersion=$(g++ --version | grep Ubuntu* | cut -d ' ' -f 4)

if [ -z "$gVersion" ]
then
	echo "g++ is not installed"

	sudo apt-get -y upgrade
	sudo apt-get -y install g++
else
	echo "g++ $gVersion detected"
	
	if [ "$gVersion" \< "4.8" ]
	then
		echo "checking for g++ updates..."
		
		sudo apt-get -y install g++ 
		gNewVersion=$(g++ --version | grep GCC* | cut -d ' ' -f 5 | cut -d '.' -f 1)
		
		if [ $gVersion \< $gNewVersion ]
		then
			sudo unlink /usr/bin/g++
			sudo ln -s /usr/bin/g++-$gNewVersion* /usr/bin/g++
		fi
	else
		echo ""
	fi

fi

echo 

echo "Checking for necessary dependencies..."
echo

#Alright, now get glfw and GLEW (freetype can be gained through the wiki as well as OpenGL).
echo "Getting other dependencies (or updating if all found)..."

#Get the necessary header files as well as doxygen, git
sudo apt-get install --yes --force-yes build-essential libtool cmake xorg-dev libxrandr-dev libxi-dev x11proto-xf86vidmode-dev libglu1-mesa-dev git libglew-dev doxygen 

echo 

#Get the glfw library
glfwFile=/usr/lib/libglfw.so

if [ -f "$glfwFile" ]
then
	echo "glfw dependency found"
else
	echo "Resolving missing glfw dependency..."
	git clone https://github.com/glfw/glfw.git || exit 1
	
	cd glfw

	#Build shared lib from source
	cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DBUILD_SHARED_LIBS=ON

	#Make and install
	make

	sudo make install

	cd ..

	sudo rm -rf glfw

	cd /usr/lib

	sudo ln -s /usr/lib/x86_64-linux-gnu/libglfw.so

	cd -
fi

#Check if we have to install freetype from source...
freetypeFile=/usr/lib/libfreetype.so

if [ -f "$freetypeFile" ]
then
	echo "freetype dependency found"
else
	echo "Resolving missing freetype dependency..."
	
	#We do, so get the freetype source
	wget downloads.sourceforge.net/project/freetype/freetype2/2.6.3/freetype-2.6.3.tar.bz2

	#Untar and unzip, configure, make, and install. 
	tar vxfj freetype-2.6.3.tar.bz2

	cd freetype-2.6.3

	./configure --prefix=/usr

	make 

	sudo make install
	
	cd ../
	
	#Remove the freetype folders from the TSGL folder
	rm -rf freetype*
fi

#Check if we have to clone cxxopts ...
cxxoptsFile=/usr/include/cxxopts.hpp

if [ -f "$cxxoptsFile" ]
then
	echo "cxxopts dependency found"
else
	echo "Resolving missing cxxopts dependency..."
	
	#We do, so clone the repository
	git clone https://github.com/jarro2783/cxxopts.git || exit 1

	#copy the necessary file 
	cd cxxopts/include

	sudo cp cxxopts.hpp /usr/include

	cd ../..
	
	#Remove the freetype folders from the TSGL folder
	rm -rf cxxopts*
fi

#check if we have to clone glm...
glmFile=/usr/include/glm/glm.hpp

if  [ -f "$glmFile" ]
then
        echo "glm dependency found"
else
        echo "Resolving missing glm dependency..."

        #clone the repository
        git clone https://github.com/g-truc/glm.git

        #copy the folder to usr/include
        cd glm

        sudo cp -r glm /usr/include

        cd ..

        rm -rf glm
fi

echo
#Dependencies were installed! (GLEW and glfw, as well as g++)
echo "All dependencies resolved!"
echo

###################################################################################

#Add export env variables to bashrc file
tsglFile=/usr/lib/libtsgl.so

if [ -f "$tsglFile" ]
then
	echo ""
else
	echo "export TSGL_HOME=/usr" >> ~/.bashrc
	echo "export TSGL_DEFAULT_FONT=/include/TSGL/assets/freefont/FreeSansBold.ttf" >> ~/.bashrc
	echo "export DISPLAY=$(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null):0" >> ~/.bashrc
	source ~/.bashrc
fi

###################################################################################

echo 
echo "Begin installation of TSGL..."
echo

#Clean install = remove the TSGL folder and lib files if they already exist
sudo rm -rf /usr/include/TSGL
sudo rm -rf /usr/lib/libtsgl.*

#Create the following directories (Since they aren't included in github but are needed)
mkdir -p lib bin

#Make clean the library
sudo make clean

#Make the library
make prefix=$PREFIX

#Install it
sudo make install prefix=$PREFIX

#Take out the .cpp files from the TSGL library package folder
sudo rm -rf /usr/include/TSGL/*.cpp

#Final step (.so file won't be found unless I do this...)
sudo ldconfig

#Done
echo "Installation complete! Execute the runtests bash script to verify that everything works!"

echo
echo
#checking update
echo "Checking for updates..."

TSGL_VERSION=$(git describe --tags --abbrev=0)
TSGL_LATEST_VERSION=$(git describe --tags $(git rev-list --tags --max-count=1))

if [ "$TSGL_VERSION" \< "$TSGL_LATEST_VERSION" ]
then
        echo "Latest version $TSGL_LATEST_VERSION found. WARNING, If you have changed anything in the TSGL folder it may be overwritten during update. To keep your changes, please commit them before updating."
        read -p "Do you want to install the update? This will replace all the files with the updated ones (y/n): " INPUT
	if [ $INPUT == y ] || [ $INPUT == Y ]
        then
                echo "Updating TSGL..."
                git remote add tsgl https://github.com/Calvin-CS/TSGL.git
                git pull tsgl master
        fi
else
        echo "Latest version already installed"
fi

