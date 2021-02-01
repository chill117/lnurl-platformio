# Changelog

* v0.1.3:
	* Encode URI components in the same way as JavaScript's encodeURIComponent. This ensures that querystrings with special characters are encoded in the same way on the device as the lnurl-node server.
* v0.1.2:
	* Follow standardization of query stringification as done by the lnurl-node module. The query object will now be sorted alphabetically by key before being stringified and used as the payload when signing.
* v0.1.1:
	* More examples and details about the library
* v0.1.0:
	* Initial release
