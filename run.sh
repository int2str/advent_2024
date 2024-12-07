#!/bin/bash

DAYS=7;

for ((day=1; day<=DAYS; ++day))
do
  ./build/${day}
done
