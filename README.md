# PizzaRD
This is an incomplete project that lets you redirect all Fortnite traffic to a custom backend of your choice (such as a self hosted LawinServer).
Please note that this only works with Fortnite versions that use WinINet instead of Curl (such as 4.5).
This project has not been widely tested, but from what i've tried it works just fine.

# How to build
Just import this into Visual Studio or any other IDE and build the Release version of this.
You can alternatively clone this GitHub repository and let GitHub actions do the trick for you.

# How to use
You have to change the REDIRECT_IP and REDIRECT_PORT constants at PizzaRD/dllmain.cpp, then inject the built DLL into Fortnite. 

# Code quality
The code in this project is supposed to "just work".
If you want to add/fix something or just improve the code please open a pull request.

# Fair use
If you're planning to use this or integrate it into your own launcher please credit me and respect the license.
If yu're an Epic Games employee and have an issue with this please open a GitHub issue.
