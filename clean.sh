#!/bin/bash

set -e

echo "Clear garbage tmp filess"

find /tmp/ -maxdepth 1 -type f -name 'myRPC*' -exec rm -f {} +

echo "Finished."
