# wheresmydialogue
A C++ program which takes an SRT file as an input and finds for a dialogue in the movie along with providing exact time instant
along with taking to that time instant in vlc.

## Assumptions:
A valid and less buggy SRT file is provided
Spelling of the words in the dialogue is correct
vlc is installed on the PC.

## Usage:

`./main filename.srt movie.(mp4/mkv/avi/flv)`

## Inside the file:
You can edit the Minimum cutoff for lines to be retrieved
`#define MATCH_CUTOFF your_required_value`

You can change the color of the matched words to RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN
