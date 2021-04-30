# Changelog

* v0.4.0:
	* Lnurl::Signer::create_signature is now a public method
* v0.3.0:
	* Refactoring and clean-up of global namespace
	* Lnurl::encode/decode are unchanged
	* BREAKING CHANGES:
		* LnurlSigner and other related globals moved to Lnurl namespace. See latest README for changes to Lnurl::Signer usage.
* v0.2.0:
	* Custom params can now be used with signer.create_url()
* v0.1.3:
	* Encode URI components in the same way as JavaScript's encodeURIComponent. This ensures that querystrings with special characters are encoded in the same way on the device as the lnurl-node server.
* v0.1.2:
	* Follow standardization of query stringification as done by the lnurl-node module. The query object will now be sorted alphabetically by key before being stringified and used as the payload when signing.
* v0.1.1:
	* More examples and details about the library
* v0.1.0:
	* Initial release
