# xif

**Twitter** will strip out metadata from your ***JPG*** image file when posted on its platform.

**xif** is a command line tool that enables you to insert tiny files (up 10KB) into your ***JPG*** image file  
in which **Twitter** will preserve, so that your data file ***"travels"*** with the image.

To maximise the amount of data you can embed, it is recommended to compress (***ZIP/RAR***, etc) your file.

*(If you want to embed more data into an image file for Twitter (up to 5MB) then please try [pdvzip](https://github.com/CleasbyCode/pdvzip)  (PNG only),  
or if you want to save your AI image prompts (Midjourney) into a JPG image file (viewable as a basic web page), then please try [imgprmt](https://github.com/CleasbyCode/imgprmt)).*

**xif** works on **Linux** and **Windows**.  

Your embedded file is stored within the ***iCC Profile*** of the ***JPG*** image file.  
*Twitter allows for only one iCC Profile with a maximum size of 10KB.*

**Video Demos**  

* [***Linux Example (RAR File)***](https://youtu.be/b9vgvgdYdVo)
* [***Windows Example (ZIP File)***](https://youtu.be/e6scfpxpCbY)
  
Compile and run the program under **Windows** or **Linux**  

## Usage (Linux Example)

```c

$ g++ xif.cpp -s -o xif
$
$ ./xif

Usage:  xif  <jpg_image>  <data_file>
        xif  --info

$ ./xif  image.jpg  sdxl_workflow_json.rar

 Created output file: "xif_img.jpg 9256 Bytes"
 You can now tweet this image.

$

```
## Extracting Your Embedded File(s)

To get access to and extract your data file for a ***ZIP*** or ***RAR*** embedded file under **Linux Desktop**, just rename 
the ***.jpg*** file extension to ***.zip***, then click the file icon to open & extract the ***ZIP/RAR*** contents.

For just a ***RAR*** file under **Linux**, you don't need to rename the file extension. From a **Linux** terminal enter the following command:

```c 
 $ unrar e xif_img.jpg
```

For an embedded ***ZIP*** file under **Windows**. Rename the ***.jpg*** file extension to ***.zip***. From a console type the following command:

```c
 PS C:\Demo> Expand-Archive  .\xif_img.zip 
```
For a ***RAR*** file under **Windows**, you will need to use a program such as **WinRar** to extract your file from the image.


My other programs you may find useful:-  

* [pdvzip - PNG Data Vehicle (ZIP Edition) for Compatible Social Media & Image Hosting Sites.](https://github.com/CleasbyCode/pdvzip)
* [imgprmt - Embed image prompts as a basic HTML page within a JPG image file](https://github.com/CleasbyCode/imgprmt)
* [jdvrif - JPG Data Vehicle for Reddit, Imgur, Flickr & Other Compatible Social Media / Image Hosting Sites.](https://github.com/CleasbyCode/jdvrif)
* [pdvrdt - PNG Data Vehicle for Mastodon & Reddit](https://github.com/CleasbyCode/pdvrdt)  
* [pdvps - PNG Data Vehicle for Twitter, PowerShell Edition](https://github.com/CleasbyCode/pdvps)   

##

