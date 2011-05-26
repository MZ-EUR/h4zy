#!/bin/bash

rm bigbitches.xpi
cd dev; zip -r bigbitches.xpi *;
cp bigbitches.xpi ../.; cd .. ;
/usr/bin/firefox bigbitches.xpi &
