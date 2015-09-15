#!/bin/bash
	
	# get the resource package
	curl -o "Resources.zip" http://cameron.tylerkline.com/Resources.zip
	unzip Resources.zip
	cp -r "Resources" "../bin/"
	
	rm -Rf Resources
	rm -Rf Resources.zip