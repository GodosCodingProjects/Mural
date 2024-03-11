
# WallPaper Animator Example folder

As of now, Mural does not have a GUI. Instead, users must setup a folder for each animation they want to use for WallPaper Animator. The structure of that project must follow rigid rules to be functional.

As a user who wishes to create an animation, start by copying this current example folder anywhere you wish. It already contains 3 useful scripts: "run.vbs", "stop.vbs" and "renameFrames.vbs". The first two allow you to start displaying the animation and to stop the executable respectively. The last one allows you to rename the frames inside the "./frames" folder (useful if their names are too long, the full path must be under 255 characters).

Next, you have to find animation frames that you want to use and place them in the "./frames" folder. You can find a gif online and split it into its frames as individual images, for example. I like to use [ezgif.com](https://ezgif.com/) for this.

Once all the frames are in the folder, it might be wise to run "./renamesFrames.vbs".

Finally, you will have to set your desired configuration in "./config.txt". The expected configuration is a 3 lines file.
- The first line contains the hexadecimal value for the filler color (in case your gif does not fill the desktop WallPaper). Note that the color is actually in BGR, instead of the usual RGB, which is RGB in little endian format.
- The second line is a word representing the WallPaper style to use (these style are the display styles defined by Windows). The list of possible styles is: center, tile, stretch, fit, fill, span
- The last line contains a decimal number, that represents the frame rate of the animation.

Now that you've set up your animation, it can simply be run with "./run.vbs".

If you want it to run at startup, you can simply add a shortcut to the "./run.vbs" script of the animation's folder inside the windows startup folder. You can access the folder easily by pressing *windows*+*R*, then entering "shell:startup".

##### [Go back to the project page](../README.md)