#!/bin/bash
if [ $# -lt 1 ]; then
	nb=10
else
	nb=$1;
fi
git log -n $nb --pretty='format:%cd %h %s'

