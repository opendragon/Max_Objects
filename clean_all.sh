#!/bin/bash
xcodebuild -scheme CommonLibrary clean -configuration Deployment
xcodebuild -scheme Device clean -configuration Deployment
xcodebuild -scheme MiscellaneousObjects clean -configuration Deployment
xcodebuild -scheme Programming clean -configuration Deployment
xcodebuild -scheme TCP clean -configuration Deployment
xcodebuild -scheme Vector clean -configuration Deployment
