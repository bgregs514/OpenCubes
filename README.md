# OpenCubes
An open source object browser for TM1/Planning Analytics

# Overview:
This project was started as a tool to learn/experiment with the GTK3 library and embedded python.  It is a work in progress, and will continue to be fine-tuned as time permits.  The overall goal of this project is to create a replacement for the IBM TM1 Architect user interface using a (hopefully) easy C base with the restAPI to facilitate data transfer from the DB.  Implementation of an easy to understand UI is key, as there are many issues with the current tools developers have at their disposal.

# If you plan to submit changes:
  - I am not an expert in all things C or Python, so please provide an explanation if the change is not obvious
  - One of the goals of this project is to keep unnecessary dependencies out of the tool - I would like to keep this as "bare bones" as possible
  
# Compiling
`make`<br />
`./browser`<br />
Upon first exection, a cubes.conf file will be created with dummy values and a message will be returned prompting the user to update the values.  This can be done by manually entering the .conf file or by using the File > Settings menu in the application itself.

# Future Plans
- Integrate TM1 Web JS library and leverage the cubeviewer
- Allow for edits to .pro files using a user-defined text editor
- More to follow
