#!/usr/bin/env sh

mkdir $1
cd $1
yt-dlp -f 160+140 $2 -o $1
ffmpeg -i `echo $1`.mp4 -r 24 $filename%d.png
rm -rf `echo $1`.mp4
