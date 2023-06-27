#!/bin/bash

cd qkd-net

# Step 4: Extract the qkd-kaiduan tar file
tar xvf qkd-kaiduan-${DEPLOYMENT_NAME}.tar

# Step 6: Move the .qkd directory to the home directory
mv .qkd ~/

# Step 7: Replace the routes.json file
cp -f /app/routes/${DEPLOYMENT_NAME}-route.json ~/.qkd/qnl/routes.json
