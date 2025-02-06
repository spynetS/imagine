# Imagine

Imagine is a small c program that renders video/images in terminal with ascii or unicode with colors!
Imagine uses ffmpeg and ffplay for media extraction and uses pipeing and system calls.

## Screenshots

![Screen1](https://github.com/spynetS/imagine/blob/main/Images/Screen2.jpg)
![Screen2](https://github.com/spynetS/imagine/blob/main/Images/Screen1.jpg)
![Screen3](https://github.com/spynetS/imagine/blob/main/Images/Screen0.jpg)

## Installation
### There is a aur package
`yay -S imagine-term-git`
### Manual
Just clone and make
`git clone https://github.com/spynetS/imagine`
`cd imagine`
`make`
to install globaly
`sudo ln -s $PWD/bin/imagine /usr/local/bin`

`

## Usage 
`imagine [options] path`


### Options

- `-t` `--type`  - specifies mode, 0 ascii low res, 1 ascii high res, 2 unicode shades, (3 pixel mode)
- `-c` `--color` - specifies the color mode, 0 no color, (1 forground), 2 background


- [ ] Mute flag
- [ ] If video and sound is not synced if video is delayed


### Author
2023 Alfred Roos
