#ifndef _SECRETS
#define _SECRETS
static char *host = (char*)"esp32";
static char *ssid = (char*)"BSides2019Badge";			 // To avoid having to enter it manually
static char *password = (char*)"BadgerHedgehogChopper"; // To avoid having to enter it manually

static const char *AsteroidsURL = "http://192.168.4.1:3000/asteroids";
static const char *AsteroidsLeaderURL = "http://192.168.4.1:3000/asteroids?_sort=score&_order=desc&_limit=5";

static const char *CryptoURL = "https://192.168.4.1:3002/crypto";
static const char *CryptoLeaderURL = "https://192.168.4.1:3002/crypto?_sort=hi_total_time&_order=asc&_limit=1";

static const char ca_cert[] = "-----BEGIN CERTIFICATE-----\n" \
"MIIDtzCCAp+gAwIBAgIUUxEct7hUGs748k9xSDj3kbSWDQ8wDQYJKoZIhvcNAQEL\n" \
"BQAwazELMAkGA1UEBhMCWkExGTAXBgNVBAgMEFdlc3Rlcm4gUHJvdmluY2UxEjAQ\n" \
"BgNVBAcMCUNhcGUgVG93bjEPMA0GA1UECgwGQlNpZGVzMRwwGgYDVQQDDBNic2lk\n" \
"ZXMyMDE5YXBpLmxvY2FsMB4XDTE5MTIwNTIwNTkyNloXDTIwMDQwMzIwNTkyNlow\n" \
"azELMAkGA1UEBhMCWkExGTAXBgNVBAgMEFdlc3Rlcm4gUHJvdmluY2UxEjAQBgNV\n" \
"BAcMCUNhcGUgVG93bjEPMA0GA1UECgwGQlNpZGVzMRwwGgYDVQQDDBNic2lkZXMy\n" \
"MDE5YXBpLmxvY2FsMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuQ+E\n" \
"LT/n4l7+uczekqB84orwAonkZLM/pkZkyM5vTY5QhhaBPKoRlgCbeZ9IXyDLq4Lk\n" \
"dkHCmFAXrBJNoJLPSW384rEGi9NglzCZSqf5DphfIm0LipSrWpMmpDoZQFLDoZEF\n" \
"zdcszPShTiL5a/tWZgolttf41fXXq2hKEsxO4ZIjIc3NjuosgcUfBoOOrOwf7bx2\n" \
"jOkj7UWGO5bhz99oFVyl1+fXr4iOmnT5rrWhUDatNRYJQoexvgy6IHPosEiaBLTj\n" \
"27Jec8RhQzHZKVw7y6zRdbHBSQ1DSlRC2CsGzhyv2A7Q6H3mx8u+kG8UX/M+Sds7\n" \
"TLQ4w10421YA0kr8YwIDAQABo1MwUTAdBgNVHQ4EFgQUPaocqN5brTPDRtPEDjRD\n" \
"F3uw0VUwHwYDVR0jBBgwFoAUPaocqN5brTPDRtPEDjRDF3uw0VUwDwYDVR0TAQH/\n" \
"BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAqqxegO2ln/gCplfYWeZCgdZ+2cMb\n" \
"KZO5CLCkoYvKKkWxiKNHT5lgJS88izwit/E9ADRVK8/evln3X1Npvt+Zxo1xAl0R\n" \
"5mvoO20kiL3+G4kEPMuvOMsNiRbvOMYyVEs1S7M2qJxqCWpGf4Nl5Rfwir4duIfh\n" \
"6VA78sE+HvJ6RwYprgS+Uumy6ILcq9E6l0CitmLUiB6eZ5gy6H3XR8MR+Zr5NaZn\n" \
"7FDQ3ZoiP7ioRbBGQy7eqy/Iz1BPcY3qpfU3i9A/noCZuoKNKIZWyvmVUJVw3HFq\n" \
"BiZreZEhsKGQObgMh43e0qrycbu7jh8KDFIB0tqkeBLyAARuQGPiOdMtIA==\n" \
"-----END CERTIFICATE-----\n";
#endif
