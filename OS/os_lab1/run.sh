#!/bin/bash

if [ $UID != 0 ]
then
	echo Please run with root
	exec sudo bash $0
fi

echo ">> Compile program" && gcc main.c -o a.out
echo ">> Running" && ./a.out
echo ">> After running" && ls -l /tmp/
echo ">> Dump content" && sudo xxd -g1 /tmp/file_owner_uid_should_be_1337
echo ">> Cleanup" && sudo rm /tmp/file_owner_uid_should_be_1337
