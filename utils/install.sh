#!/usr/bin/env bash

ln -s ../../src/ build/aces/src
mkdir webots/worlds webots/controllers
git clone git://dasl.mem.drexel.edu/vrHubo.git webots/worlds/

mkdir webots/controllers/minihubo webots/controllers/hubo
ln -s ../../../build/aces/bin/aces-launcher webots/controllers/minihubo/minihubo
ln -s ../../../src/orocosScripts/minihubo-webots-walk.ops webots/controllers/minihubo/dispatcher.ops
ln -s ../../../src/orocosScripts/walkPatterns/ webots/controllers/minihubo/walkPatterns

#ln -s ../../../build/aces/bin/aces-launcher webots/controllers/hubo/hubo
#ln -s ../../../src/orocosScripts/hubo.ops webots/controllers/hubo/dispatcher.ops
#ln -s ../../../src/orocosScripts/walkPatterns/ webots/controllers/hubo/walkPatterns
