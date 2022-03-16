#!/bin/bash
set -e
rad_path=/"${1}"/radiation-benchmarks
echo "-- Creating ${rad_path} dir"
set -x
sudo mkdir -p "${rad_path}"/log

chmod ugo+w "${rad_path}"
chmod ugo+w "${rad_path}"/log
