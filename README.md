# Nintendo 3DS EBook Reader
[Download](https://github.com/reworks/3DS_eBook_Reader/releases/latest "Download the latest release here.")

## Instructions:
Place books in a folder named "books" on the *root* of your SD card. Some books can take a while to load! Be patient!

Use calibre (google it) to convert other formats to .epub. Make sure its as strict as possible when converting.

MUST BE IN A VALID AND COMPLIANT .EPUB FORMAT. DRM-FREE EBOOK SHOPS ARE YOUR BEST BET.

IF THE FORMAT IS WRONG, THE APP WILL FREEZE/CRASH.

## Building
You must first install devkitPro, libctru, sf2d, sftd, sfil and the portlibs. To get the zip on windows you will also need gnu32 zip + unzip binaries.

Then run "build-cia.bat" or "build-cia.sh" depending on your platform.

## Credits:
- AlbertoSonic for initial codebase and graphics.
- cots for the new fancy artwork.
- xerpi for SF2D, SFIL & SFTD
- minizip for unzipping
- tinyxml2 for xml parsing
- BLUnzip for simplified C++ minizip
- zlib for zip functions
- freetype2 for font manipulation
- litehtml for html parsing and rendering.
- The people at #cakey
- some random guy on the internet: helping me get litehtml working properly.
- Source Code Pro font: https://github.com/adobe-fonts/source-code-pro

## License:
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
