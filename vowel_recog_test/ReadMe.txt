Assignment: Vowel recognition
Roll number:204101047

Note:This file still does not work as expected.
I took the reference values of C[i] from my friend, as mine were not accurate.
This code does the following:

Records 2 second silence, and calculates DC shift.
Takes input of the name of test file[Please bring test file to this folder. Only 5 test files were brought to this folder to remove clutter]
Opens the test file and applies DC shift on all it's values. This is stored in DC_adjusted.txt
Opens DC_adjusted.txt, applies normalization, and saves it Normalized.txt
Opens Normalized.txt, and saves 1600 samples[5 frames] in buffer, and eventually to samples[5][320]
For each Frame, calculates the Ri's, Ai's and Ci's. Ci's are stored in final_C.txt
Then, we open reference files one by one, calculate Tokhura distance between them, and save it in final_dist[5]
Choose the minimum distance, and according to the position, give output.



========================================================================
    CONSOLE APPLICATION : vowel_recog_test Project Overview
========================================================================

AppWizard has created this vowel_recog_test application for you.

This file contains a summary of what you will find in each of the files that
make up your vowel_recog_test application.


vowel_recog_test.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

vowel_recog_test.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

vowel_recog_test.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named vowel_recog_test.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
