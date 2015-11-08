#!/bin/bash
xcodebuild -scheme CommonLibrary -configuration Deployment
xcodebuild -scheme Device -configuration Deployment
xcodebuild -scheme MiscellaneousObjects -configuration Deployment
xcodebuild -scheme Programming -configuration Deployment
xcodebuild -scheme TCP -configuration Deployment
xcodebuild -scheme Vector -configuration Deployment
