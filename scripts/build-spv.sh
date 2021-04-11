#!/usr/bin/env bash

name=$1

glslc res/vk-shader/$name.frag -o res/vk-shader/$name.frag.spv
glslc res/vk-shader/$name.vert -o res/vk-shader/$name.vert.spv
