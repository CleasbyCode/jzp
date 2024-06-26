# jzp

When posted on its platform, **X/Twitter** will remove the metadata from your image file.

**jzp** is a command-line tool used to embed small files (up to 10KB) into a ***JPG*** image,  
that will not be removed by **X/Twitter**. Your embedded data file ***"travels"*** with the image.

To maximise the amount of data you can embed, it is recommended to compress (***ZIP/RAR***) your data file.  

![Demo Image_a](https://github.com/CleasbyCode/xif/blob/main/demo_image/bottle1.jpg)   
***[{Original image source}](https://comfyanonymous.github.io/ComfyUI_examples/sdxl/)***

Compile and run the program under **Windows** or **Linux**  

## Usage (Linux Example)

```console

$ g++ main.cpp -O2 -s -o jzp
$
$ ./jzp

Usage: jzp <cover_image> <data_file>
       jzp --info

$ ./jzp image.jpg sdxl_workflow_json.rar

 Created output file: "jzp_img.jpg 9256 Bytes"

 You can now post this data-embedded image file on X/Twitter.

```
## Extracting Your Embedded File(s)

To get access to and extract your data file for a ***ZIP*** or ***RAR*** embedded file with a **Linux desktop**,  
just rename the ***.jpg*** file extension to ***.zip***, then click the file icon to open & extract the ***ZIP/RAR*** contents.

For just a ***RAR*** file under **Linux**, you don't need to rename the file extension. From a **Linux** terminal enter the following command:

```console
 $ unrar e jzp_img.jpg
```

For an embedded ***ZIP*** file under **Windows**, rename the ***.jpg*** file extension to ***.zip***.  
From a console type the following command:

```console
 PS C:\Demo> Expand-Archive .\jzp_img.zip .
```
For a ***RAR*** file under **Windows**, you will need to use a program such as **WinRar** to extract your file from the image.

My other programs you may find useful:-  

* [pdvzip: CLI tool to embed a ZIP file within a tweetable and "executable" PNG-ZIP polyglot image.](https://github.com/CleasbyCode/pdvzip)
* [jdvrif: CLI tool to encrypt & embed any file type within a JPG image.](https://github.com/CleasbyCode/jdvrif)
* [imgprmt: CLI tool to embed an image prompt (e.g. "Midjourney") within a tweetable JPG-HTML polyglot image.](https://github.com/CleasbyCode/imgprmt)
* [pdvrdt: CLI tool to encrypt, compress & embed any file type within a PNG image.](https://github.com/CleasbyCode/pdvrdt)
* [pdvps: PowerShell / C++ CLI tool to encrypt & embed any file type within a tweetable & "executable" PNG image](https://github.com/CleasbyCode/pdvps)

##

