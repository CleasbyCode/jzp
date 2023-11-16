# jzp

**Twitter** will strip out metadata from your image file when posted on its platform.

**jzp** is a simple command-line tool that will insert small files (up to 10KB) into a ***JPG*** image file,  
in which **Twitter** will preserve, so that your data file ***"travels"*** with the image.

*For compatibility reasons, please use a JPG image from Twitter.*

*If you want to use a JPG image not currently from Twitter, first post it to Twitter,  
click the image to fully expand it, save the image. You can now use this Twitter encoded JPG image with jzp.*

To maximise the amount of data you can embed, it's recommended to compress (***ZIP/RAR***) your data file.  

![Demo Image_a](https://github.com/CleasbyCode/xif/blob/main/demo_image/bottle.jpg)   
***[{Original image source}](https://comfyanonymous.github.io/ComfyUI_examples/sdxl/)***


Your embedded file is stored within the ***ICC Profile*** of the ***JPG*** image file.  

![Demo Image_b](https://github.com/CleasbyCode/xif/blob/main/demo_image/jzp_icc.png)  

[***Video Demo***](https://youtu.be/SIMZe5Ix5Y8)

Compile and run the program under **Windows** or **Linux**  

## Usage (Linux Example)

```console

$ g++ jzp.cpp -O2 -s -o jzp
$
$ ./jzp

Usage:  jzp <jpg_image> <data_file>
        jzp --info

$ ./jzp image.jpg sdxl_workflow_json.rar

 Created output file: "jzp_img.jpg 9256 Bytes"
 You can now post this data-embedded JPG image on Twitter.

```
## Extracting Your Embedded File(s)

To get access to and extract your data file for a ***ZIP*** or ***RAR*** embedded file under **Linux Desktop**, just rename 
the ***.jpg*** file extension to ***.zip***, then click the file icon to open & extract the ***ZIP/RAR*** contents.

For just a ***RAR*** file under **Linux**, you don't need to rename the file extension. From a **Linux** terminal enter the following command:

```console
 $ unrar e jzp_img.jpg
```

For an embedded ***ZIP*** file under **Windows**, rename the ***.jpg*** file extension to ***.zip***. From a console type the following command:

```console
 PS C:\Demo> Expand-Archive  .\jzp_img.zip .
```
For a ***RAR*** file under **Windows**, you will need to use a program such as **WinRar** to extract your file from the image.

My other programs you may find useful:-  

* [pdvzip: CLI tool to embed a ZIP file within a tweetable and "executable" PNG-ZIP polyglot image.](https://github.com/CleasbyCode/pdvzip)
* [jdvrif: CLI tool to encrypt & embed any file type within a JPG image.](https://github.com/CleasbyCode/jdvrif)
* [imgprmt: CLI tool to embed an image prompt (e.g. "Midjourney") within a tweetable JPG-HTML polyglot image.](https://github.com/CleasbyCode/imgprmt)
* [pdvrdt: CLI tool to encrypt, compress & embed any file type within a PNG image.](https://github.com/CleasbyCode/pdvrdt)
* [pdvps: PowerShell / C++ CLI tool to encrypt & embed any file type within a tweetable & "executable" PNG image](https://github.com/CleasbyCode/pdvps)

##

