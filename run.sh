#!/bin/bash

DAYS=8;

for ((day=1; day<=DAYS; ++day))
do
  ./build/${day}
done
