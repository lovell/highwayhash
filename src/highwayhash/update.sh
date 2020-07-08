#!/bin/bash

for filename in *.{cc,h}; do
  curl -sO https://raw.githubusercontent.com/google/highwayhash/master/highwayhash/$filename
done
