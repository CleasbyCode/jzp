# Xif

***Twitter*** will strip out metadata from your *JPG* image file when posted on its platform.

***xif*** is a command line tool that enables you to insert tiny files (up 10KB) into your *JPG* image file  
in which Twitter will preserve, so your data file travels with the image.

To maximise the amount of data you can embed, it is recommended to compress (ZIP/RAR, etc) your file.

*(If you want to embed more data into an image file for Twitter (up to 5MB) then please try [pdvzip](https://github.com/CleasbyCode/pdvzip)  (PNG only),  
or if you want to save your AI image prompts into a JPG image file (tweetable & viewable as a basic web page), then please try [imgprmt](https://github.com/CleasbyCode/imgprmt)).*

***xif*** works on Linux and Windows.  

Your embedded file is stored within the *iCC Profile* of the *JPG* image file.  
*Twitter allows for only one iCC Profile with a maximum size of 10KB.*

**Video Demos**  

* [***Windows Example***](https://youtu.be/hBupOu7fbfo)
* [***Linux Example***](https://youtu.be/BfvwBWeAfzM)
  
Compile and run the program under Windows or **Linux**  

## Usage (Linux Example)

```c

$ g++ xif.cpp -s -o xif
$
$ ./xif

Usage:  xif  <jpg_image>  <data_file>
        xif  --info

$ ./xif  image.jpg  my_source_code.rar

 Created output file: "xif_img.jpg 9256 Bytes"
 You can now tweet this image.

$

```
## Extracting Your Embedded File(s)

***First Linux*** option. To get access to and extract your embedded data file(s) using Linux, just rename the *.jpg*  
file extension to *.zip*, and then from the Linux desktop, click the file icon to open & extract the ZIP contents. 

***Second Linux*** option. From the terminal, use the following commands:

```c 
 $ zip -FF your_image.jpg --out extract.zip
 $ unzip extract.zip
```

***Third Linux option***, if you have problems with the first and second options.

```c
 $ 7z e your_image.jpg
```

***First Windows option***. Rename the *.jpg* file extension to *.cmd*, then from a console type the following command: 

```c
 PS C:\Demo> .\your_image.cmd 
```
You can also open the *.cmd* file from the Windows desktop, but you will get a secruity warning on its first run.  
Clear the warning by clicking '***More info***' then select '***Run anyway***'.  

***Second Windows*** option. Rename the *.jpg* file extension to *.zip*, then from a console type the following command: 

```c
 PS C:\Demo> Expand-Archive  .\your_image.zip 
```

My other programs you may find useful:-  

* [pdvzip - PNG Data Vehicle (ZIP Edition) for Compatible Social Media & Image Hosting Sites.](https://github.com/CleasbyCode/pdvzip)
* [imgprmt - Embed image prompts as a basic HTML page within a JPG image file](https://github.com/CleasbyCode/imgprmt)
* [jdvrif - JPG Data Vehicle for Reddit, Imgur, Flickr & Other Compatible Social Media / Image Hosting Sites.](https://github.com/CleasbyCode/jdvrif)
* [pdvrdt - PNG Data Vehicle for Reddit](https://github.com/CleasbyCode/pdvrdt)  
* [pdvps - PNG Data Vehicle for Twitter, PowerShell Edition](https://github.com/CleasbyCode/pdvps)   

##

