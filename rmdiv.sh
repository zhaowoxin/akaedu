#!/bin/bash

kill -3 `ps -aux | grep emulator | awk -F' ' '{print $2}'`
