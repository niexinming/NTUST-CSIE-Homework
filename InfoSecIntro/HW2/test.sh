#!/bin/sh

key=abababababababab
iv=fbfbfbfbfbfbfbfb

if [ ! -x des ]
then
	echo "[+] Compiling ./des"
	make
else
	echo "[*] ./des existed"
fi

infile="test.bmp"
for operation in enc dec
do
	for mode in ECB CBC CTR OFB
	do
		if [ $operation = dec ]
		then
			infile=enc-$mode.bmp
			outfile=dec-$mode.bmp
		else
			outfile=enc-$mode.bmp
		fi
		cmd="./des $infile $outfile $operation $mode $key $iv"
		echo "[+] Running $cmd"
		$cmd
	done
done
