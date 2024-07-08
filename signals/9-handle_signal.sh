#!/bin/sh

# Setting signal handlers to trap
trap '"echo Nope; exit"' SIGINT