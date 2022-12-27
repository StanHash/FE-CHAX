#!/bin/bash

VERSION=$(date +"%Y%m%d")
DIR=CHaxInstallReady-$VERSION

make hack
rm -rf $DIR
mkdir $DIR
cp -r Wizardry $DIR
cd $DIR
find -type f -name '*.o' -delete
rm -rf Wizardry/.Limbo Wizardry/.Research Wizardry/3rdParty
