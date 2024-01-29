# Imagine

Imagine is a small c program that renders images in terminal
with ascii or unicode with colors!
Imagine is built with the stb librarys.

## Screenshots

![Screen1](https://github.com/spynetS/imagine/blob/main/Images/Screen2.jpg)
![Screen2](https://github.com/spynetS/imagine/blob/main/Images/Screen1.jpg)
![Screen3](https://github.com/spynetS/imagine/blob/main/Images/Screen0.jpg)

## Usage 
`imagine [options] path`

### Path is file
 If the path is a image imagine will render it in the terminal

### Path is folder
if the path is a folder imagine will render all png files inside
with a 24 images per second. So you have extracted frames from a video
imagine will render the video.

- there is a script `download.sh` [foldername] [url] which will download and
extract the frames from a yt url


### Options

- `-t` `--type`  - specifies mode, 0 ascii low res, 1 ascii high res, (2 unicode)
- `-c` `--color` - specifies the color mode, 0 no color, (1 forground), 2 background


## TODO
- [ ] stream videos


### Athor
2023 Alfred Roos
