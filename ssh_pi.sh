#!/bin/bash

#Based on script written by Jake Ammons

#Set USER to username on the rpi
USER="pi"

#Navigate to Git repo, pull updates, and connect to the latest ip address
git pull
echo "connecting to $USER@$(cat ip_address)"
ssh $USER@$(cat ip_address)

