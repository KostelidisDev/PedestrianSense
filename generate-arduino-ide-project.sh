#!/usr/bin/env sh

PROJECT_NAME="PedestrianSense"

rm -fr ./arduino-ide/${PROJECT_NAME}/*
mkdir -p ./arduino-ide/${PROJECT_NAME}

cp ./src/main.cpp ./arduino-ide/${PROJECT_NAME}/${PROJECT_NAME}.ino
sed -i '' '/#include <Arduino.h>/d' "./arduino-ide/${PROJECT_NAME}/${PROJECT_NAME}.ino"

cp ./README.md ./arduino-ide/${PROJECT_NAME}/README.md