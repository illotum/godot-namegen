#!/bin/sh
pushd ./godot-cpp
scons godotbinpath="godot" generate_bindings=yes
popd
scons
