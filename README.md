# UE4 Data Validator Plugin for UI assets

Very much a work-in-progress.

Current validation applied:

* All Texture2D assets must be a multiple of 4 (to [support
  compression](https://benui.ca/unreal/ui-texture-size/), if needed).
* The Data Source folder must be set in Editor Settings.
* All Teture2D assets must be imported from a path inside the Data Source
  folder (to aid [reimporting between team members](https://benui.ca/unreal/reimporting-assets/)).

## License

[CC0](https://creativecommons.org/publicdomain/zero/1.0/)

## Contact

If you find it useful, drop me a line [@_benui](https://twitter.com/_benui on Twitter)

[benui.ca](https://benui.ca)
