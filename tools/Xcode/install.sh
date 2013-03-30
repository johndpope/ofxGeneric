#!/bin/bash
#

XcodeLocation="/Applications/Xcode.app"

echo "Starting ofxGeneric Xcode Tools Install Process"

# ofxGeneric Code Snippets
echo "Installing Code Snippets"
/bin/mkdir -p ~/Library/Developer/Xcode/UserData/CodeSnippets/
/bin/cp -R -f CodeSnippets/* ~/Library/Developer/Xcode/UserData/CodeSnippets/ 

# ofxGeneric File Templates
echo "Installing File Templates"
mkdir ~/Library/Developer/Xcode/Templates
/bin/cp -R -f File\ Templates/* ~/Library/Developer/Xcode/Templates