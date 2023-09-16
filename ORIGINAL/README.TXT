Main README.TXT file for "Tricks of the 3D Game Programming Gurus - Advanced 3D Graphics and Rasterization"
Created: 1.1.03

------------

License Agreement

CD-ROM and software copyright (C) 2003 Sams Publishing & Pearson Education Inc., 
Andre' LaMothe, Xtreme Games LLC, Nurve Networks LLC. All rights reserved. 
Individual programs are copyrighted by their respective owners and may require 
separate licensing. This CD-ROM may not be redistributed without prior written 
permission from the publisher. The right to redistribute the individual programs 
on the CD-ROM depends on each program's license. Consult each program for details.

------------

Table of Contents:

I.   INTRODUCTION
II.  INSTALLING DIRECTX
III. INSTALLING THE SOURCE CODE FROM THE BOOK
IV.  INSTALLING THE TOOLS
V.   PROBLEMS YOU MIGHT ENCOUNTER (PLEASE READ!!!!!!)
VI.  COMPATIBILITY.

I. INTRODUCTION

   Welcome to the wonderful world of Tricks II! On this CD is a
   plethora of digital treasures from source code, games, and DirectX, to
   applications, and electronic bonus books on 3D, Direct3D, and general topics.

   The first thing you might notice is that isn't any kind of main installation
   program on the CD. I have found that 9 out of 10 times the best installer is
   the reader since only he/she knows what and where to install things, so I'm
   going to leave the actuall file copying and installation to you.

   However, within each directory there is a README.TXT file that explains what
   the files are and what they are for. The CD is set up like this:

   T3DIICD <DIR> - The main directory/CD root.
      |
      \DIRECTX       <DIR> - Contains DirectX SDK.
      \ARTICLES      <DIR> - Contains 3D articles by other authors.
      \TOOLS         <DIR> - Contains all the tools and applications.
      \MEDIA         <DIR> - Contains stock art/sound media for your use.
      \SOURCE        <DIR> - Contains the entire source code for the book.
      \GAMES         <DIR> - Some 3D software based game(s).      
      \EBOOK         <DIR> - Appendixes A through F in PDF format.

II. INSTALLING DIRECTX

     To run the DirectX programs from this book and to create your own, you
     must have the DirectX 8.1 (or better) Run-Time system and Software
     Development Kit (SDK) loaded. Both the DirectX 9.0 Run-Time files 
     and the SDK are contained on this CD within the DIRECTX\ directory
     which is the newest DirectX version when this book went to print.

     The README.TXT within the DIRECTX\ sub-directory will give you more
     details on the setup and installation of DirectX, but basically you
     must first install the run-time files and then the SDK. The SDK is
     needed by your compiler, so it can access the header, and library
     files needed to build DirectX applications.


III. INSTALLING THE SOURCE CODE FROM THE BOOK

     The source code and data for each chapter of this book are contained
     within the SOURCE\ sub-directory. I suggest simply copying the entire
     SOURCE\ directory as-is onto your hard drive by "dragging" it from
     the this CD or by using XCOPY or other similar technique. You may
     need to unzip the contents of each directory, if so make sure to
     enable directories.
     
IV.   INSTALLING THE APPLICATIONS

     There are a number of awesome applications that are contained on this
     CD such as TrueSpace, Paint Shop Pro, WinZip, sound editors, 3D tools, 
     and Acrobat Reader which you need to read anything in .PDF format.

     Each one of these applications installs as a self extracting .EXE and
     takes just a couple minutes. Take a look at the README.TXT file for
     each application for details.

V.   PROBLEMS YOU MIGHT ENCOUNTER

     * Read Only Flag *

     This is a very important detail, so read on. When creating a CD ROM disk
     all the files will be written with the READ-ONLY flag enabled. This is 
     fine in most cases unless you copy the files to your hard drive (which you will)
     and then edit and try to write the files back to disk. You will get a 
     READ-ONLY protection error. 

     Fixing this is a snap. You simply need to clear the READ-ONLY flag on any 
     files that you want to modify. There are 3 ways to do it. First, you can 
     do it with Windows and simply navigate into the directory with the file 
     you want to clear the READ-ONLY flag and then select the file, press 
     the RIGHT mouse button to get the file properties and then clear the 
     READ-ONLY flag and APPLY you change. You can do this will more than one 
     file at once by selecting a group of files.

     The second way is to use the File Manager and perform a similar set of 
     operations as in the example above. The third and best way is to the DOS 
     ATTRIB command with a DOS prompt. Here's how: 

     Assume you have copied the entire SOURCE directory on your hard drive to the 
     location C:\SOURCE. To reset all the READ-ONLY attributes in one fell 
     swoop you can use the ATTRIB command. Here is the syntax:
    
     C:\SOURCE\ATTRIB -r *.* /s

     This says to clear the READ-ONLY flag "r" from all files "*.*" and all lower 
     sub-directories "/s".
                       
     * DirectX Driver Problems *

     DirectX may not have a driver for your video or sound card. If this is the case, 
     you will be notified during installation. But don't worry if DirectX doesn't 
     install a driver, it will still work, just not as good. If this happens you'll 
     have to go to the manufacturer of your video or sound card and look for DirectX 
     drivers, or else, keep an eye out at the Microsoft DirectX site for new updates.
 

     * Compilation Problems *

     99% of compiler problems are "pilot" errors. So before you think that something 
     is wrong, make sure that you have all your DirectX library, and header paths 
     set up in the compiler to point to the DirectX SDK directories as the first in
     in the search list. Make sure that you are creating a standard Win32 .EXE 
     (if you are making a DirectX app). Make sure you have manually included the 
     DirectX .LIB files in your link list or you have included them in your source
     tree. And finally, make sure that you can compile a basic "hello world" program
     with Visual C++. Don't jump into DirectX Windows programming without getting 
     your compiler and environment setup properly. 

     * Video Problems *

     There are some video cards that even with a DirectX driver, still don't work 
     correctly. This will appear as a sudden flash of black or white video
     when you try to run some of the demos. If this happens, try going into the
     display properties and reseting your machine's video to the demos desired 
     resolution and bit depth, for example: 640x480 in 256 colors, or 24-bit mode, 
     etc. Also, you MUST have your desktop set to 16-bit mode for the 16-bit 
     windowed mode demos since they can't change video modes and must use the 
     current desktop mode. 

VI.  COMPATIBILITY

     The contents of this book were tested on Windows 98/ME/XP/2000 Pentium II, III, IV, 
     and equivalent AMD processors. I highly recommend at least a Pentium III 1.0Ghz or
     better since the demos are software accelerated only. Additionally, I suppose most
     of the software will work on Windows 95. In general, you need:

     Pentium III+ @ 1.0Ghz, Pentium 4 @ 1.5+ Ghz recommended.
     64 MB RAM, 128 MB recommended.
     500 megs of free disk space.
     2D graphics accelerator.
     16-bit sound card.


2003 The year of the the MaTriX...
Lord Necron



