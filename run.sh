#!/bin/bash

DAYS=9;

for ((day=1; day<=DAYS; ++day))
do
  ./build/${day}
done
